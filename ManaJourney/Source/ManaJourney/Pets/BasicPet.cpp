// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "../DamageTypes/MeleeDamageType.h"
#include "../DamageTypes/ManaDamageType.h"
#include "Kismet/KismetMathLibrary.h"
#include "BasicPet.h"

float ABasicPet::ATTACKING_STATE = 0.0f;
ABasicPet::ABasicPet()
{
	// location can be adjusted depending on the weapon
	// NOTE Enable collision here
	MeleeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCapsule"));
	MeleeCapsule->AttachParent = GetMesh();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ConstructorHelpers::FObjectFinder<UParticleSystem> attackParticle(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Troll/Impact/P_TrollSwingImpact_Poison.P_TrollSwingImpact_Poison"));
	attackEffect = attackParticle.Object;

	
}

// Called every frame
void ABasicPet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (focusedEnemy && !focusedEnemy->bIsDead)
	{
		FaceEnemy();
	}

}

void ABasicPet::AssignOwner(ACharacter* owningActor)
{
	petOwner = (owningActor) ? owningActor : UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

bool ABasicPet::ShouldAttack()
{
	bool bshouldAttack = false;
	AManaJourneyCharacter* manaJourneyCharacter = Cast<AManaJourneyCharacter>(petOwner);
	if (manaJourneyCharacter)
	{
		bshouldAttack = (manaJourneyCharacter->enemyFound == nullptr) ? false : true;
	}
	
	return bshouldAttack;
}

ACharacter* ABasicPet::GetPetOwner()
{
	return petOwner;
}

void ABasicPet::FaceEnemy()
{
	const FVector petLocation = GetActorLocation();
	const FVector enemyLocation = focusedEnemy->GetActorLocation();
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(petLocation, enemyLocation);
	SetActorRotation(FMath::Lerp(GetActorRotation(), lookRotation, 0.2f));
}


void ABasicPet::ReleaseEnemy()
{
	if (focusedEnemy)
	{
		focusedEnemy = nullptr;
	}
}

void ABasicPet::InitAttack(ABasicEnemy* enemyToAttack)
{

	focusedEnemy = enemyToAttack;
	DamageTargetedEnemy(enemyToAttack);
	this->StopWalking();
	meleeAttackBP(strength, ATTACKING_STATE);
	
}

void ABasicPet::DamageTargetedEnemy(ABasicEnemy* enemyToAttack)
{
	TArray<AActor*> AllActors;
	AActor* actor;
	ABasicEnemy* enemy;
	MeleeCapsule->GetOverlappingActors(AllActors, ABasicEnemy::StaticClass());

	float damageAmount = strength;
	// iterate over all actors found
	for (int32 i = 0; i < AllActors.Num(); i++)
	{
		actor = AllActors[i];
		enemy = Cast<ABasicEnemy>(actor);
		if (!MeleeDamage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MeleeDamage not set::: resulting to default")));
			MeleeDamage = UDamageType::StaticClass();
		}
		if (enemy && GetController() && MeleeDamage && damageAmount != 0.0f && enemy == enemyToAttack && !enemy->bIsDead)
		{
			UGameplayStatics::ApplyDamage(enemyToAttack, damageAmount, GetController(), this, MeleeDamage);
			// TODO HANDLE sockes for different attacking states
			// own mesh
			FTimerHandle AttackingTimer;
			GetWorldTimerManager().ClearTimer(AttackingTimer);
			FTimerDelegate attackDelegate = FTimerDelegate::CreateUObject(this, &ABasicPet::InvokeMeleeAttackEffect, enemyToAttack->GetMesh(), 1.0f);
			GetWorldTimerManager().SetTimer(AttackingTimer, attackDelegate, 1.0f, false);
		}
	}
}

void ABasicPet::InvokeMeleeAttackEffect(USkeletalMeshComponent* meshToAttach, float attackingState)
{
	FName socketToSpawnMeleeEffect = "Head";

	if (attackingState == 1.0f) { socketToSpawnMeleeEffect = "SlapEffect_FirstAttack"; }

	// spawns effect at sword
	attackEffectComp = UGameplayStatics::SpawnEmitterAttached(attackEffect, meshToAttach, socketToSpawnMeleeEffect, FVector(0, 0, 0), FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true);

	
	FTimerHandle AttackingTimerDestroy;
	GetWorldTimerManager().ClearTimer(AttackingTimerDestroy);
	FTimerDelegate destroyEffectDelegate = FTimerDelegate::CreateUObject(this, &ABasicPet::DestroyEffect, attackEffectComp);
	GetWorldTimerManager().SetTimer(AttackingTimerDestroy, destroyEffectDelegate, 0.3f, false);
}

void ABasicPet::DestroyEffect(UParticleSystemComponent* effect2Destroy)
{
	if (effect2Destroy)
	{
		effect2Destroy->DestroyComponent();
	}
}


void ABasicPet::TakeCharacterDamage(float Damage, UDamageType* damageType, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (!bIsDead)
	{
		health = health - Damage;
		if (health <= 0.0f)
		{
			handleDeath();
		}
		else
		{
			//	handleDamageText(Damage, DamageCauser);
			handleHitByDamageType(DamageCauser, damageType);
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("you are dead")));
	}

}
void ABasicPet::handleHitByDamageType(AActor* DamageCauser, UDamageType* damageType)
{
	UManaDamageType* manaDamageType;
	// use the timer provided by the damage type
	manaDamageType = Cast<UManaDamageType>(damageType);
	if (manaDamageType)
	{
		/*
		FTimerHandle CastingTimer;
		GetWorldTimerManager().ClearTimer(CastingTimer);
		GetWorldTimerManager().SetTimer(CastingTimer, FTimerDelegate::CreateUObject(this, &ABasicEnemy::handleHit), manaDamageType->fTakeDamageTime, false);
		*/
	}
	else
	{
		handleHit();
		// on melee attacks we use push back (currently this push back is not depending on melee damage type, but this is a possible implementation)
		//pushBackByDamageCauser(100000.0f, DamageCauser);
	}

}

void ABasicPet::handleHit()
{
	this->receiveHitAnimation();
}


void ABasicPet::handleDeath()
{
	bIsDead = true;
	this->StopMeleeAttack();
	this->PlayDeathEffect();

}

void ABasicPet::Ressurection()
{
	bIsDead = false;
	health = maxHealth;

}