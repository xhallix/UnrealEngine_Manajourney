// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Staff.h"
#include "Weapon/DragonSword.h"
#include "MeleeCombatSystem.h"




// Called every frame but enable tick node in BP
void AMeleeCombatSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	clearAttackingState();

}


void AMeleeCombatSystem::clearAttackingState()
{
	// no attack after certain time, reset attack
	// if the currentAttackState does not change over certain time, reset it
	seconds++;
	if (seconds == 100.0f)
	{
		storedAttackState = currentAttackState;
	}

	if (seconds == 300.0f)
	{
		if (currentAttackState == storedAttackState)
		{
			currentAttackState = 0.0f;
		}
		seconds = 0.0f;
	}
}


void AMeleeCombatSystem::setUserReference(AManaJourneyCharacter* characterToUse)
{
	if (characterToUse)
	{
		characterReference = characterToUse;
		setAnimationTimeFrame();
	} 
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No characterToUse provided for MeleeCombatSystem")));
	}
}


void AMeleeCombatSystem::init()
{
	if (characterReference)
	{
	
		if (canStartNextAttackingState)
		{
			updateAttackingState();
		}

		if (!isMeleeAttackOngoing)
		{
			float damage = characterReference->strength;
			invokeMeleeCombat(damage);
		}
	}
	
}

void AMeleeCombatSystem::setAnimationTimeFrame()
{
	if (characterReference)
	{
		animationFrames = characterReference->meleeAttackSequences;
		if (animationFrames.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No animationFrames for the character set ::: defaulting to 1.0f")));
			//provision the array
			animationFrames.Add(1.0f);
			animationFrames.Add(1.0f);
			animationFrames.Add(1.0f);
		}
	}
}



void AMeleeCombatSystem::invokeAttackingStates()
{
	setTimeFrames();
	freeAttackingState();
	FTimerHandle ResetAttackHandler;
	GetWorldTimerManager().ClearTimer(ResetAttackHandler);
	GetWorldTimerManager().SetTimer(ResetAttackHandler, FTimerDelegate::CreateUObject(this, &AMeleeCombatSystem::blockAttackingState), timeFrameForAttackingState + 0.3, false);
}


void AMeleeCombatSystem::invokeMeleeCombat(float damageAmount)
{
	
	if (characterReference)
	{

		isMeleeAttackOngoing = true;
		// animation start in blueprints
		characterReference->meleeAttackBP(damageAmount, currentAttackState);
		characterReference->GetCharacterMovement()->Deactivate();
		setTimeFrames();

		float attackingEffectDelay = 0.2f; // time to wait until the particleeffect occurs which indicates an attack 

		if (currentAttackState == 1.0f) { attackingEffectDelay = 0.2f; }
		if (currentAttackState == 2.0f) { attackingEffectDelay = 0.2f; }
		if (currentAttackState == 3.0f) { attackingEffectDelay = 0.6f; }
		

		// timer to wait when the next attack can start
		FTimerHandle ResetAttackHandler;
		GetWorldTimerManager().ClearTimer(ResetAttackHandler);
		GetWorldTimerManager().SetTimer(ResetAttackHandler, FTimerDelegate::CreateUObject(this, &AMeleeCombatSystem::canStartNewMeleeAttack), timeFrameToNewMeleeAttack, false);

		// timer to show effect on weapon
		FTimerHandle WeaponImpactHandler;
		GetWorldTimerManager().ClearTimer(WeaponImpactHandler);
		GetWorldTimerManager().SetTimer(WeaponImpactHandler, FTimerDelegate::CreateUObject(this, &AMeleeCombatSystem::handleWeaponEffects, 0.2f), attackingEffectDelay, false);

		// timer to invoke attack method which checks the melee capsule 
		FTimerHandle DamageImpactHandler;
		GetWorldTimerManager().ClearTimer(DamageImpactHandler);
		GetWorldTimerManager().SetTimer(DamageImpactHandler, FTimerDelegate::CreateUObject(this, &AMeleeCombatSystem::damageTargetedEnemy), attackingEffectDelay, false);	
	} 
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No characterReference provided for MeleeCombatSystem ::: please use setReference before calling this")));
	}
}

void AMeleeCombatSystem::updateAttackingState()
{

	if (shoulResetAttackingStates || currentAttackState >= 3.0f)
	{
		currentAttackState = 1.0f;
	}
	else
	{
		currentAttackState++;
	}

	blockAttackingState();

	float timeToNewMeleeAttack = 0.2f;	// ( depends on the animation time )
	if (currentAttackState == 1.0f) { timeToNewMeleeAttack = animationFrames[0]; }
	if (currentAttackState == 2.0f) { timeToNewMeleeAttack = animationFrames[1]; }
	if (currentAttackState == 3.0f) { timeToNewMeleeAttack = animationFrames[2]; }
	// timer to wait when the next attack can start
	FTimerHandle ResetAttackHandler;
	GetWorldTimerManager().ClearTimer(ResetAttackHandler);
	GetWorldTimerManager().SetTimer(ResetAttackHandler, FTimerDelegate::CreateUObject(this, &AMeleeCombatSystem::freeAttackingState), timeToNewMeleeAttack, false);

}


// only use when weaponreference exists
void AMeleeCombatSystem::handleWeaponEffects(float time)
{
	if (characterReference)
	{
		characterReference->weaponReference->handleAttackEffect(time, characterReference->GetMesh(), currentAttackState);
	}

}

void AMeleeCombatSystem::canStartNewMeleeAttack()
{
	isMeleeAttackOngoing = false;
	characterReference->GetCharacterMovement()->Activate();
}


void AMeleeCombatSystem::blockAttackingState()
{
	canStartNextAttackingState = false;
}

void AMeleeCombatSystem::freeAttackingState()
{
	canStartNextAttackingState = true;
}

void AMeleeCombatSystem::setTimeFrames()
{
	if (currentAttackState == 1.0f) { timeFrameToNewMeleeAttack = timeFrameForAttackingState = animationFrames[0];/*1.5f;*/ }
	if (currentAttackState == 2.0f) { timeFrameToNewMeleeAttack = timeFrameForAttackingState = animationFrames[1];/* 1.6f;*/ }
	if (currentAttackState == 3.0f) { timeFrameToNewMeleeAttack = timeFrameForAttackingState = animationFrames[2];/*1.3f;*/ }
}

void AMeleeCombatSystem::damageTargetedEnemy()
{
	TArray<AActor*> AllActors;
	AActor* actor;
	ABasicEnemy* enemy;
	characterReference->MeeleCapsule->GetOverlappingActors(AllActors, ABasicEnemy::StaticClass());

	//Random damage amount
	float damageAmount = FMath::RandRange(characterReference->strength, characterReference->strength + 5);

	// iterate over all actors found
	for (int32 i = 0; i < AllActors.Num(); i++)
	{
		actor = AllActors[i];
		enemy = Cast<ABasicEnemy>(actor);
		if (!characterReference->MeleeDamage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MeleeDamage not set::: resulting to default")));
			characterReference->MeleeDamage = UDamageType::StaticClass();
		}
		if (enemy && 
			characterReference->playerController && characterReference->player && characterReference->MeleeDamage 
			&& damageAmount != 1.0f && enemy == characterReference->enemyFound 
			&& !enemy->bIsDead)
		{
			UGameplayStatics::ApplyDamage(enemy, damageAmount, characterReference->GetController(), this, characterReference->MeleeDamage);
			if (characterReference->weaponReference)
			{
				characterReference->weaponReference->meshToHit = enemy->GetMesh();
			}
		}
	}
}

