// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../ManaJourneyCharacter.h"
#include "../Pets/BasicPet.h"
#include "../Combat/MeleeCombatSystem.h"
#include "../ManaClasses/BlackMage.h"
#include "../DamageTypes/MeleeDamageType.h"
#include "../DamageTypes/ManaDamageType.h"
#include "BasicEnemy.h"


// Sets default values
float ABasicEnemy::ATTACKING_STATE = 0.0f;
ABasicEnemy::ABasicEnemy(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bSkippedTarget = false;

	MeleeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCapsule"));
	MeleeCapsule->AttachParent = GetMesh();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Create the TargetIndicator
	FocusComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("TargetIndicator"));
	FocusComponent->AttachParent = GetMesh();
	FocusComponent->SetHiddenInGame(true);

	DamageTextComponent = ObjectInitializer.CreateDefaultSubobject<UTextRenderComponent>(this, TEXT("DamageText"));
	DamageTextComponent->AttachParent = GetMesh();	
	DamageTextComponent->SetRelativeLocationAndRotation(initialDamageTextLocation, initialDamageTextRotation);
	DamageTextComponent->SetHiddenInGame(true);


}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	bSkippedTarget = false;
	bInitialised = true;
}

void ABasicEnemy::TakeCharacterDamage(float Damage, UDamageType* damageType, class AController* EventInstigator, class AActor* DamageCauser)
{
	
	if (!bIsDead)
	{
		enemyHealth = enemyHealth - Damage;	
		if (enemyHealth <= 0.0f)
		{
			// not casting anything so handle death immediatly
			if (!damageType->IsA(UManaDamageType::StaticClass()) && !damageType->IsA(UMeleeDamageType::StaticClass()))
			{
				handleDeath(DamageCauser);
			} 
			else
			{
				handleDeathByDamageType(DamageCauser, damageType);
			}
		}
		else 
		{
			handleDamageText(Damage, DamageCauser);
			handleHitByDamageType(DamageCauser, damageType);
		}

	}
	
}


void ABasicEnemy::handleDeath(AActor* DamageCauser)
{
	this->StopAttacking();
	this->StopWalking();
	this->PlayDeathEffect();
	disableMenu();
	bIsDead = true;

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AManaJourneyCharacter* enemyPlayer = Cast<AManaJourneyCharacter>(player);
	AManaJourneyCharacter* attacker = Cast<AManaJourneyCharacter>(DamageCauser);
	ABasicPet* enemyPet = Cast<ABasicPet>(DamageCauser);

	if (enemyPlayer)
	{
		enemyPlayer->disableTargetSystem();
		enemyPlayer->gainExp(exp);
	}


	

}

void ABasicEnemy::handleDeathByDamageType(AActor* DamageCauser, UDamageType* damageType)
{
	UManaDamageType* manaDamageType;
	UMeleeDamageType* meleeDamageType;
	// use the timer provided by the damage type
	manaDamageType = Cast<UManaDamageType>(damageType);
	meleeDamageType = Cast<UMeleeDamageType>(damageType);

	// use either manaDamageType
	if (manaDamageType)
	{
		FTimerHandle CastingTimer;
		GetWorldTimerManager().ClearTimer(CastingTimer);
		FTimerDelegate destroyEffectDelegateChar = FTimerDelegate::CreateUObject(this, &ABasicEnemy::handleDeath, DamageCauser);
		GetWorldTimerManager().SetTimer(CastingTimer, destroyEffectDelegateChar, manaDamageType->fTakeDamageTime,  false);
	}
	else if (meleeDamageType)
	{
		// or we have meleeDamageType
		/*
		FTimerHandle CastingTimer;
		GetWorldTimerManager().ClearTimer(CastingTimer);
		FTimerDelegate destroyEffectDelegateChar = FTimerDelegate::CreateUObject(this, &ABasicEnemy::handleDeath, DamageCauser);
		GetWorldTimerManager().SetTimer(CastingTimer, destroyEffectDelegateChar, meleeDamageType->fTakeDamageTime, false);
		*/
		handleDeath(DamageCauser);
	} 
	else
	{
		handleDeath(DamageCauser);
	}
}

void ABasicEnemy::handleHitByDamageType(AActor* DamageCauser, UDamageType* damageType)
{
	UManaDamageType* manaDamageType;
	// use the timer provided by the damage type
	manaDamageType = Cast<UManaDamageType>(damageType);
	if (manaDamageType) 
	{
		FTimerHandle CastingTimer;
		GetWorldTimerManager().ClearTimer(CastingTimer);
		GetWorldTimerManager().SetTimer(CastingTimer, FTimerDelegate::CreateUObject(this, &ABasicEnemy::handleHit), manaDamageType->fTakeDamageTime, false);
	} 
	else
	{
		handleHit();
		// on melee attacks we use push back (currently this push back is not depending on melee damage type, but this is a possible implementation)
		pushBackByDamageCauser(100000.0f, DamageCauser);
	}
	
}

void ABasicEnemy::handleHit()
{
	this->StopWalking();
	this->StopAttacking();
	this->receiveHitAnimation();	
}



// Called every frame
void ABasicEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	// make the focus component move
	FocusComponent->AddWorldRotation(FRotator(0, 0.7, 0));

	if (attacker && !bIsDead)
	{
		FaceAttacker();
	}

}

void ABasicEnemy::FaceAttacker()
{
	const FVector currentLocation = this->GetActorLocation();
	const FVector attackerLocation = attacker->GetActorLocation();
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(currentLocation, attackerLocation);
	SetActorRotation(FMath::Lerp(GetActorRotation(), lookRotation, 0.2f));
}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ABasicEnemy::toggleFocus()
{
	if (bFocused)
	{
		bFocused = false;
	}
	else
	{
		bFocused = true;
	}
}

void ABasicEnemy::setFocus()
{
	if (!IsPendingKill())
	{
		bFocused = true;
		this->enableMenu();
	}
	
}

void ABasicEnemy::removeFocus()
{
	if (!IsPendingKill())
	{
		bFocused = false;
	}
}

void ABasicEnemy::showFocusComponent()
{
	if (FocusComponent && FocusComponent != nullptr && bInitialised)
	{
		FocusComponent->SetHiddenInGame(false);
	}
}

void ABasicEnemy::hideFocusComponent()
{
	FocusComponent->SetHiddenInGame(true);
}

void ABasicEnemy::hideDamageTextComponent()
{
	DamageTextComponent->SetHiddenInGame(true);
}

void ABasicEnemy::destroyEffect(UParticleSystemComponent* effect2Destroy)
{
	if (effect2Destroy)
	{
		effect2Destroy->DestroyComponent();
	}
}

void ABasicEnemy::handleDamageText(float damageAmount, AActor* DamageCauser)
{
	float distanceToDamageCauser = DamageCauser->GetDistanceTo(this);
	float scaleToSet = (distanceToDamageCauser * 0.004) + 1.0;
	//DamageTextComponent
	DamageTextComponent->SetRelativeScale3D(FVector(scaleToSet, scaleToSet, scaleToSet));
	DamageTextComponent->SetHiddenInGame(false);
	const FString damageAmountString = FString::FromInt(damageAmount);
	
	DamageTextComponent->SetText(FText::FromString(damageAmountString));
	moveDamageText(FVector(0, 0, 200), initialDamageTextRotation, initialDamageTextLocation, initialDamageTextRotation, initialDamageTextScale);

}

// if this does not work check:
// CapsuleComponent -> GenerateOverlappingEvents
void ABasicEnemy::pushBackByDamageCauser(float pushBackPower, AActor* DamageCauser)
{
	AMeleeCombatSystem* meleeCombatSystemInitiator = Cast<AMeleeCombatSystem>(DamageCauser);
	ABasicPet* petInitiatior = Cast<ABasicPet>(DamageCauser);

	if (meleeCombatSystemInitiator && !bIsDead)
	{
	
		if (meleeCombatSystemInitiator->characterReference)
		{
			UCapsuleComponent* capsuleComponent = meleeCombatSystemInitiator->characterReference->GetCapsuleComponent();
			FRotator componentRotation = capsuleComponent->GetComponentRotation();
			FVector rotationVector = UKismetMathLibrary::Conv_RotatorToVector(componentRotation);
			FVector pushBackVector = UKismetMathLibrary::Multiply_VectorFloat(rotationVector, pushBackPower);
			GetCharacterMovement()->AddImpulse(pushBackVector);
		}
	}

	if (petInitiatior)
	{
		UCapsuleComponent* capsuleComponent = petInitiatior->GetCapsuleComponent();
		FRotator componentRotation = capsuleComponent->GetComponentRotation();
		FVector rotationVector = UKismetMathLibrary::Conv_RotatorToVector(componentRotation);
		FVector pushBackVector = UKismetMathLibrary::Multiply_VectorFloat(rotationVector, pushBackPower);
		GetCharacterMovement()->AddImpulse(pushBackVector);
	}

}

void ABasicEnemy::InitMeleeAttack()
{
	
	if (attacker && !bIsDead)
	{
		ABasicPet* pet = Cast<ABasicPet>(attacker);
		AManaJourneyCharacter* manaChar = Cast<AManaJourneyCharacter>(attacker);
		if (manaChar)
		{ 
			if (!manaChar->bIsDead)
			{
				AttackManaChar();
			}
			
		}
		if (pet)
		{
			if (!pet->bIsDead)
			{
				AttackPet();
			}
			else 
			{
				StopAttacking();
			}
			
		}
	}	

}


void ABasicEnemy::AttackManaChar()
{
	if (!bIsDead)
	{
		DamageEnemy(attacker);
		this->UseAttackEffect(attacker);
		this->StopWalking();
		this->meleeAttackBP(strength, ATTACKING_STATE);
	}

}

void ABasicEnemy::AttackPet()
{
	if (!bIsDead)
	{
		DamageEnemy(attacker);
		this->UseAttackEffect(attacker);
		this->meleeAttackBP(strength, ATTACKING_STATE);
	}

}



void ABasicEnemy::DamageEnemy(AActor* enemyToAttack)
{
	TArray<AActor*> AllPlayerAttackerActors;
	TArray<AActor*> AllPetAttackerActors;
	AActor* playerActor;
	AActor* petActor;
	ABasicPet* petChar;
	AManaJourneyCharacter* manaJourneyChar;
	MeleeCapsule->GetOverlappingActors(AllPetAttackerActors, ABasicPet::StaticClass());
	MeleeCapsule->GetOverlappingActors(AllPlayerAttackerActors, AManaJourneyCharacter::StaticClass());

	float damageAmount = strength;
	// iterate over all PLAYER actors found
	for (int32 i = 0; i < AllPlayerAttackerActors.Num(); i++)
	{
		playerActor = AllPlayerAttackerActors[i];
		manaJourneyChar = Cast<AManaJourneyCharacter>(playerActor);
		if (!MeleeDamage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MeleeDamage not set::: resulting to default")));
			MeleeDamage = UDamageType::StaticClass();
		}
		if (manaJourneyChar && GetController() && MeleeDamage && damageAmount != 0.0f)
		{
			UGameplayStatics::ApplyDamage(manaJourneyChar, damageAmount, GetController(), this, MeleeDamage);
		}
	}

	// Now check if the PET is also affected
	for (int32 i = 0; i < AllPetAttackerActors.Num(); i++)
	{
		petActor = AllPetAttackerActors[i];
		petChar = Cast<ABasicPet>(petActor);
		if (!MeleeDamage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MeleeDamage not set::: resulting to default")));
			MeleeDamage = UDamageType::StaticClass();
		}
		if (petChar && GetController() && MeleeDamage && damageAmount != 0.0f)
		{
		
			UGameplayStatics::ApplyDamage(petChar, damageAmount, GetController(), this, MeleeDamage);
		}
	}

}
