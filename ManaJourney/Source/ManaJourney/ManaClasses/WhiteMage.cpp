// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "../ManaJourneyCharacter.h"
#include "WhiteMage.h"


// Sets default values
AWhiteMage::AWhiteMage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UParticleSystem> shell1particle(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Ability/Armor/P_Reduced_Melee_Shield_01.P_Reduced_Melee_Shield_01"));
	shell1particleEffect = shell1particle.Object;
	ConstructorHelpers::FObjectFinder<UParticleSystem> shell2particle(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Skill_Aura/P_Aura_Default_Upheaval_01.P_Aura_Default_Upheaval_01"));
	shell2particleEffect = shell2particle.Object;
	
	ConstructorHelpers::FObjectFinder<UParticleSystem> cure1particle2(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Ability/Summon/P_Summon_Portal.P_Summon_Portal"));
	cure1particleEffect = cure1particle2.Object;
	ConstructorHelpers::FObjectFinder<UParticleSystem> cure2particle2(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Cines/Ausar/P_Ausar_Resurrection.P_Ausar_Resurrection"));
	cure2particleEffect = cure2particle2.Object;

	// TODO - implement effects for next level
	cure1particleEffect2 = cure1particleEffect;
	cure2particleEffect2 = cure2particleEffect;
	shell1particleEffect2 = shell1particleEffect;
	shell2particleEffect2 = shell2particleEffect;

	
	ConstructorHelpers::FObjectFinder<UParticleSystem> invisibleparticle(TEXT("/Game/marketplace-packs/Vectorfields/Particles/P_Thaw.P_Thaw"));
	invisibleparticleEffect = invisibleparticle.Object;
}

// Called when the game starts or when spawned
void AWhiteMage::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWhiteMage::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWhiteMage::initCureSpells(float level)
{
	if (level == 1)
	{
		delayCureParticle1 = cure1particleEffect;
		delayCureParticle2 = cure2particleEffect;
	}

	if (level > 1)
	{
		delayCureParticle1 = cure1particleEffect2;
		delayCureParticle2 = cure2particleEffect2;
	}


}

void AWhiteMage::initShellSpells(float level)
{
	if (level == 1)
	{
		delayShellParticle1 = shell1particleEffect;
		delayShellParticle2 = shell2particleEffect;
	}

	if (level > 1)
	{
		delayShellParticle1 = shell1particleEffect2;
		delayShellParticle2 = shell2particleEffect2;
	}

}

AActor* AWhiteMage::getActor()
{
	return this;
}

void  AWhiteMage::levelUp(float level)
{
}

bool AWhiteMage::castCure(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh)
{
	float manaCosts = character.level * 1.5;
	bool bcureCasted = false;
	initCureSpells(character.level);
	// only cast spell if we have enough mana and no other spell ongoing
	if (character.mana > manaCosts && character.health < character.maxHealth && referenceCharacterMovement && referenceCharacterMovement->IsActive())
	{
		if (character.level <= 5)
		{
			if (character.health += 20 > character.maxHealth)
			{
				character.health = character.maxHealth;
			}
			else 
			{
				character.health += 20;
			}
			
			character.mana -= manaCosts;
		}

		if (character.level  > 5 && character.level  < 10)
		{
			if (character.health += 40 > character.maxHealth)
			{
				character.health = character.maxHealth;
			}
			else
			{
				character.health += 40;
			}
			character.mana -= manaCosts;
		}

		castCureEffect(mesh);
		bcureCasted = true;
	}
	
	return bcureCasted;
}

void AWhiteMage::castCureEffect(USkeletalMeshComponent* mesh)
{
	if (referenceCharacterMovement && referenceCharacterMovement->IsActive())
	{
		referenceCharacterMovement->StopActiveMovement();
		referenceCharacterMovement->Deactivate();
		commonCureParticleComponent1 = UGameplayStatics::SpawnEmitterAttached(delayCureParticle1, mesh, NAME_None, FVector(-10, 10, -40), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
		commonCureParticleComponent2 = UGameplayStatics::SpawnEmitterAttached(delayCureParticle2, mesh, NAME_None, FVector(-10, 10, -40), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);

		FTimerHandle CureCasting;
		GetWorldTimerManager().ClearTimer(CureCasting);
		GetWorldTimerManager().SetTimer(CureCasting, FTimerDelegate::CreateUObject(this, &AWhiteMage::destroyEffect, commonCureParticleComponent1), 1.50f, false);
		
		FTimerHandle CureCasting2;
		GetWorldTimerManager().ClearTimer(CureCasting2);
		GetWorldTimerManager().SetTimer(CureCasting2, FTimerDelegate::CreateUObject(this, &AWhiteMage::destroyEffect, commonCureParticleComponent2), 1.50f, false);
	}
}

bool AWhiteMage::castInvisible(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh)
{
	float manaCosts = character.level * 1.5;
	bool binvisibleCasted = false;
	// only cast spell if we have enough mana and no other spell ongoing
	if (character.mana > manaCosts && referenceCharacterMovement && referenceCharacterMovement->IsActive())
	{
		character.mana -= manaCosts;
		castInvisibleEffect(mesh);
		binvisibleCasted = true;
	}
	return binvisibleCasted;
}

void AWhiteMage::castInvisibleEffect(USkeletalMeshComponent* mesh)
{
	if (referenceCharacterMovement)
	{
		toggleVisibility(mesh);
		invisibleparticleEffectComp = UGameplayStatics::SpawnEmitterAttached(invisibleparticleEffect, mesh, NAME_None, FVector(-10, 10, 0), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
		float invisbleEffectDuration = 7.00f;

		// timer for particle effect destruction
		FTimerHandle InvisibleEffect;
		GetWorldTimerManager().ClearTimer(InvisibleEffect);
		FTimerDelegate destroyEffectDelegate = FTimerDelegate::CreateUObject(this, &AWhiteMage::destroyEffect, invisibleparticleEffectComp);
		GetWorldTimerManager().SetTimer(InvisibleEffect, destroyEffectDelegate, invisbleEffectDuration, false);

		// makes the character visible again
		FTimerHandle InvisibleVisibility;
		GetWorldTimerManager().ClearTimer(InvisibleVisibility);
		FTimerDelegate toggleVisibility = FTimerDelegate::CreateUObject(this, &AWhiteMage::toggleVisibility, mesh);
		GetWorldTimerManager().SetTimer(InvisibleVisibility, toggleVisibility, invisbleEffectDuration, false);

	}
}

bool AWhiteMage::castShell(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh)
{
	float level = character.level;
	float manaCosts = level * 1.5;
	initShellSpells(level);
	bool bShellCasted = false;
	// only cast spell if we have enough mana and no other spell ongoing
	if (character.mana > manaCosts && referenceCharacterMovement && referenceCharacterMovement->IsActive())
	{
		character.mana -= manaCosts;
		castShellEffect(mesh);
		bShellCasted = true;
		bHasShell = true;
	}

	return bShellCasted;
}

void AWhiteMage::castShellEffect(USkeletalMeshComponent* mesh)
{
	if (referenceCharacterMovement && referenceCharacterMovement->IsActive())
	{

		commonShellParticleComponent1 = UGameplayStatics::SpawnEmitterAttached(delayShellParticle1, mesh, NAME_None, FVector(-10, 10, 0), FRotator(0, 0, 0), EAttachLocation::SnapToTargetIncludingScale, true);
		commonShellParticleComponent2 = UGameplayStatics::SpawnEmitterAttached(delayShellParticle2, mesh, NAME_None, FVector(-10, 10, 0), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
		
		// shell casting
		FTimerHandle ShellCasting;
		GetWorldTimerManager().ClearTimer(ShellCasting);
		FTimerDelegate destroyEffectDelegate = FTimerDelegate::CreateUObject(this, &AWhiteMage::destroyEffect, commonShellParticleComponent1);
		GetWorldTimerManager().SetTimer(ShellCasting, destroyEffectDelegate, fShellTimer, false);

		// shell remove
		FTimerHandle ShellRemove;
		GetWorldTimerManager().ClearTimer(ShellRemove);
		FTimerDelegate shellRemoveDelegate = FTimerDelegate::CreateUObject(this, &AWhiteMage::setShellFalse);
		GetWorldTimerManager().SetTimer(ShellRemove, shellRemoveDelegate, fShellTimer, false);

	}
}


void AWhiteMage::destroyEffect(UParticleSystemComponent* effect2Destroy)
{
	if (referenceCharacterMovement && !referenceCharacterMovement->IsActive())
	{
		referenceCharacterMovement->Activate();
	}
	if (effect2Destroy)
	{
		effect2Destroy->DestroyComponent();
	}
}

void AWhiteMage::toggleVisibility(USkeletalMeshComponent* mesh)
{
	if (mesh->IsVisible())
	{
		mesh->SetHiddenInGame(true, true);
	}
	else
	{
		mesh->SetHiddenInGame(false, true);
	}
	
}


void AWhiteMage::setShellFalse()
{
	bHasShell = false;
}