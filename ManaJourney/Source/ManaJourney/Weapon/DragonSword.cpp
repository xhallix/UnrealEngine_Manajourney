// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "DragonSword.h"



ADragonSword::ADragonSword()
{
	ConstructorHelpers::FObjectFinder<UParticleSystem> attackParticle(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_SwipeTrail_Mesh.P_SwipeTrail_Mesh"));
	attackEffect = attackParticle.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> impactParticle1(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Mobile/Impacts/P_Impact_Enemy_Base.P_Impact_Enemy_Base"));
	impactEffect1 = impactParticle1.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> impactParticle2(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Mobile/combat/P_YoYo_Target_Beam.P_YoYo_Target_Beam"));
	impactEffect2 = impactParticle2.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> specialParticle(TEXT("/Game/epic-packs/InfinityBladeEffects/Effects/FX_Combat_Base/WeaponCombo/P_Combo_Finish_Fire_01.P_Combo_Finish_Fire_01"));
	specialAttackEffect = specialParticle.Object;

}



void ADragonSword::handleAttackEffect(float effectTime, USkeletalMeshComponent* mesh, float attackingStateToUse)
{
	// spawns effect at sword
	attackEffectComp = UGameplayStatics::SpawnEmitterAttached(attackEffect, mesh, "MeleeEffects_DragonSword", FVector(0, 0, 0), FRotator(100, 0, 0), EAttachLocation::SnapToTarget, true);

	FTimerHandle AttackingTimer;
	GetWorldTimerManager().ClearTimer(AttackingTimer);
	FTimerDelegate destroyEffectDelegate = FTimerDelegate::CreateUObject(this, &Super::destroyEffect, attackEffectComp);
	GetWorldTimerManager().SetTimer(AttackingTimer, destroyEffectDelegate, effectTime, false);


	if (meshToHit)
	{
		FTimerHandle ImpactTimer;
		GetWorldTimerManager().ClearTimer(ImpactTimer);
		FTimerDelegate releaseEffectDelegate = FTimerDelegate::CreateUObject(this, &ADragonSword::releaseImpactEffect, meshToHit, 0.0f);
		GetWorldTimerManager().SetTimer(ImpactTimer, releaseEffectDelegate, effectTime - 0.2, false);
	}

	// special effect here
	if (attackingStateToUse >= 3.0f)
	{
		specialAttackComp = UGameplayStatics::SpawnEmitterAttached(specialAttackEffect, mesh, "MeleeEffects_DragonSword", FVector(0, 0, 0), FRotator(100, 0, 0), EAttachLocation::SnapToTarget, true);
	}
}

void ADragonSword::releaseImpactEffect(USkeletalMeshComponent* currentMeshToHit, float currentAttackingState)
{
	impactEffectComp1 = UGameplayStatics::SpawnEmitterAttached(impactEffect1, currentMeshToHit, NAME_None, FVector(0, 0, 0), FRotator(100, 0, 0), EAttachLocation::SnapToTarget, true);
	impactEffectComp2 = UGameplayStatics::SpawnEmitterAttached(impactEffect2, currentMeshToHit, NAME_None, FVector(0, 20, 60), FRotator(75, 50, 0), EAttachLocation::SnapToTargetIncludingScale, false);

	FTimerHandle ImpactTimer;
	GetWorldTimerManager().ClearTimer(ImpactTimer);
	FTimerDelegate destroyEffectDelegate = FTimerDelegate::CreateUObject(this, &Super::destroyEffect, impactEffectComp2);
	GetWorldTimerManager().SetTimer(ImpactTimer, destroyEffectDelegate, 0.1f, false);

	// disable mesh so this is not used when we do sword attack but have not enemy hit
	meshToHit = nullptr;
}
