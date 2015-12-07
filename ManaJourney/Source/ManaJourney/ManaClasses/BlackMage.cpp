// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "BlackMage.h"


// Sets default values
ABlackMage::ABlackMage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;

	// delay destroy
	ConstructorHelpers::FObjectFinder<UParticleSystem> fire1particle(TEXT("/Game/marketplace-packs/Vectorfields/Particles/P_GroundFire.P_GroundFire"));
	fire1particleEffect = fire1particle.Object;
	ConstructorHelpers::FObjectFinder<UParticleSystem> fire1particle2(TEXT("/Game/marketplace-packs/Vectorfields/Particles/P_GroundFire.P_GroundFire"));
	fire1particleEffect2 = fire1particle.Object;

	// auto destroy
	ConstructorHelpers::FObjectFinder<UParticleSystem> fire2particle(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Elemental/Fire/P_Elemental_Leap_Impact_Fire.P_Elemental_Leap_Impact_Fire"));
	fire2particleEffect = fire2particle.Object;
	ConstructorHelpers::FObjectFinder<UParticleSystem> fire2particle2(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Ausar/P_AU_ShieldSlam_AOE.P_AU_ShieldSlam_AOE"));
	fire2particleEffect2 = fire2particle2.Object;


	/////////////////// TORNADO ////////////////////////

	// delay destroy
	ConstructorHelpers::FObjectFinder<UParticleSystem> tornado1particle1(TEXT("/Game/marketplace-packs/Vectorfields/Particles/P_Tornado.P_Tornado"));
	tornado1particleEffect = tornado1particle1.Object;
	ConstructorHelpers::FObjectFinder<UParticleSystem> tornado1particle2(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Skill_RockBurst/P_RBurst_Lightning_Burst_Area_01.P_RBurst_Lightning_Burst_Area_01"));
	tornado2particleEffect = tornado1particle2.Object;
	

	// auto destroy
	ConstructorHelpers::FObjectFinder<UParticleSystem> tornado2particle1(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Skill_RockBurst/P_RBurst_Default_Charge_Slam_01.P_RBurst_Default_Charge_Slam_01"));
	tornado2particleEffect2 = tornado2particle1.Object;
	ConstructorHelpers::FObjectFinder<UParticleSystem> tornado2particle2(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Skill_RockBurst/P_RBurst_Lightning_Burst_Stalag_01.P_RBurst_Lightning_Burst_Stalag_01"));
	tornado2particleEffect2 = tornado2particle2.Object;


	//////////////// BIND ///////////////////
	ConstructorHelpers::FObjectFinder<UParticleSystem> bind1particle(TEXT("/Game/marketplace-packs/Vectorfields/Particles/P_EventHorizon.P_EventHorizon"));
	bind1particleEffect = bind1particle.Object;
	ConstructorHelpers::FObjectFinder<UParticleSystem> particleChar(TEXT("/Game/marketplace-packs/Vectorfields/Particles/P_Suction.P_Suction"));
	particleEffectCharacter = particleChar.Object;

}

// Called when the game starts or when spawned
void ABlackMage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackMage::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}





void ABlackMage::levelUp(float level)
{

}

void ABlackMage::castInvokerEffect(AManaJourneyCharacter* character)
{
	if (referenceCharacterMovement && referenceCharacterMovement->IsActive())
	{
		referenceCharacterMovement->StopActiveMovement();
		referenceCharacterMovement->Deactivate();
		// spawns effect at invoker (character)
		particleEffectCompCharacter = UGameplayStatics::SpawnEmitterAttached(particleEffectCharacter, character->GetMesh(), NAME_None, FVector(0, 0, 35), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
		FTimerHandle CastingChar;
		GetWorldTimerManager().ClearTimer(CastingChar);
		FTimerDelegate destroyEffectDelegateChar = FTimerDelegate::CreateUObject(this, &ABlackMage::destroyEffect, particleEffectCompCharacter);
		GetWorldTimerManager().SetTimer(CastingChar, destroyEffectDelegateChar, character->animationTimer, false);
	}
}


void ABlackMage::castSpellEffect(AManaJourneyCharacter* character, USkeletalMeshComponent* enemyMesh, UParticleSystemComponent* particleComponent, UParticleSystem* particleSystem, float destroyTimer)
{
	// spawns effect at enemy
	if (enemyMesh && particleSystem)
	{
		particleComponent = UGameplayStatics::SpawnEmitterAttached(particleSystem, enemyMesh, NAME_None, FVector(0, 0, 100), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
		if (particleComponent && enemyMesh)
		{
			FTimerHandle Casting;
			GetWorldTimerManager().ClearTimer(Casting);
			FTimerDelegate destroyEffectDelegate = FTimerDelegate::CreateUObject(this, &ABlackMage::destroyEffect, particleComponent);
			GetWorldTimerManager().SetTimer(Casting, destroyEffectDelegate, destroyTimer, false);
		}

	}
}

bool ABlackMage::castFire(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh)
{
	bool bfireCasted = false;
	float level = character.level;
	float manaCosts = level * 1.2;
	float damageAmount = level * 2.5 + 70.0f;
	ABasicEnemy* enemyToAttack = character.enemyFound;
	AManaJourneyCharacter* referenceCharacter = &character;
	AActor* currentActor = Cast<AActor>(referenceCharacter);

	initFireSpells(level);

	// fallback in cast of missing damage
	if (!FireDamage)
	{
		FireDamage = UDamageType::StaticClass();
	}

	if (currentActor && character.mana > manaCosts && referenceCharacterMovement && referenceCharacterMovement->IsActive() && enemyToAttack && !enemyToAttack->IsPendingKill() && FireDamage)
	{
		character.mana -= manaCosts;
		UGameplayStatics::ApplyDamage(enemyToAttack, damageAmount, character.GetController(), currentActor, FireDamage);
		castInvokerEffect(referenceCharacter);
		FTimerHandle Casting;
		
		GetWorldTimerManager().ClearTimer(Casting);
		FTimerDelegate effectDelegate = FTimerDelegate::CreateUObject(this, &ABlackMage::castSpellEffect, referenceCharacter, enemyToAttack->GetMesh(), commonFireParticleComponent, delayFireParticle, 2.50f);
		GetWorldTimerManager().SetTimer(Casting, effectDelegate, 1.00, false);
		bfireCasted = true;

		// auto destroyes
		UGameplayStatics::SpawnEmitterAttached(autoFireParticle, enemyToAttack->GetMesh(), NAME_None, FVector(-10, 10, 20), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
	}
	
	return bfireCasted;
}

void ABlackMage::initFireSpells(float level)
{
	if (level == 1)
	{
		delayFireParticle = fire1particleEffect;
		autoFireParticle = fire2particleEffect;
	}
	if (level > 1)
	{
		delayFireParticle = fire1particleEffect2;
		autoFireParticle = fire2particleEffect2;
	}
}


void ABlackMage::initAirSpells(float level)
{
	if (level == 1)
	{
		delayTornadoParticle = tornado1particleEffect;
		autoTornadoParticle = tornado2particleEffect;
	}
	if (level > 1)
	{

		delayTornadoParticle = tornado1particleEffect2;
		autoTornadoParticle = tornado2particleEffect2;
	}
}

bool ABlackMage::castTornado(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh)
{
	bool btornadoCasted = false;
	float level = character.level;
	float manaCosts = level * 1.2f + 7.0f;
	float damageAmount = level * 2.5f + 50.f ;
	ABasicEnemy* enemyToAttack = character.enemyFound;
	AManaJourneyCharacter* referenceCharacter = &character;
	AActor* currentActor = Cast<AActor>(referenceCharacter);

	initAirSpells(level);

	// fallback in cast of missing damage
	if (!TornadoDamage)
	{
		TornadoDamage = UDamageType::StaticClass();
	}

	if (currentActor && character.mana > manaCosts && referenceCharacterMovement && referenceCharacterMovement->IsActive() && enemyToAttack && TornadoDamage)
	{
		character.mana -= manaCosts;
		UGameplayStatics::ApplyDamage(enemyToAttack, damageAmount, character.Controller, currentActor, TornadoDamage);
		castInvokerEffect(referenceCharacter);

		FTimerHandle Casting;
		GetWorldTimerManager().ClearTimer(Casting);
		FTimerDelegate effectDelegate = FTimerDelegate::CreateUObject(this, &ABlackMage::castSpellEffect, referenceCharacter, enemyToAttack->GetMesh(), commonTornadoParticleComponent, delayTornadoParticle, 2.50f);
		GetWorldTimerManager().SetTimer(Casting, effectDelegate, 1.00, false);
		btornadoCasted = true;

		UGameplayStatics::SpawnEmitterAttached(autoTornadoParticle, enemyToAttack->GetMesh(), NAME_None, FVector(-10, 10, 30), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);


	}

	return btornadoCasted;
}



bool ABlackMage::castBind(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh)
{
	bool bCasted = false;

	float level = character.level;
	float manaCosts = level * 1.2f + 5.0f;
	ABasicEnemy* enemyToAttack = character.enemyFound;
	AManaJourneyCharacter* referenceCharacter = &character;

	if (character.mana > manaCosts && referenceCharacterMovement && referenceCharacterMovement->IsActive() && enemyToAttack)
	{
		character.mana -= manaCosts;
		castInvokerEffect(referenceCharacter);
		toggleCharacterMovement(enemyToAttack->GetCharacterMovement());

		FTimerHandle Casting;
		GetWorldTimerManager().ClearTimer(Casting);
		FTimerDelegate effectDelegate = FTimerDelegate::CreateUObject(this, &ABlackMage::castSpellEffect, referenceCharacter, enemyToAttack->GetMesh(), bind1particleEffectComp, bind1particleEffect, 5.0f);
		GetWorldTimerManager().SetTimer(Casting, effectDelegate, 1.00f, false);

		FTimerHandle MovementHandle;
		GetWorldTimerManager().ClearTimer(MovementHandle);
		FTimerDelegate movementDelegate = FTimerDelegate::CreateUObject(this, &ABlackMage::toggleCharacterMovement, enemyToAttack->GetCharacterMovement());
		GetWorldTimerManager().SetTimer(MovementHandle, movementDelegate, 5.00f, false);


		bCasted = true;
	}

	return bCasted;
}



void ABlackMage::destroyEffect(UParticleSystemComponent* effect2Destroy)
{
	if (referenceCharacterMovement && !referenceCharacterMovement->IsActive())
	{
		referenceCharacterMovement->Activate();
	}
	if (effect2Destroy && effect2Destroy->IsActive() )
	{
		effect2Destroy->DestroyComponent();
	}

}

void ABlackMage::toggleCharacterMovement(UCharacterMovementComponent* characterMovementComponent)
{
	if (characterMovementComponent->IsActive())
	{
		characterMovementComponent->Deactivate();
	}
	else
	{
		characterMovementComponent->Activate();
	}

}
