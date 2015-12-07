// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/BasicWeapon.h"
#include "DragonSword.generated.h"

/**
 * 
 */
UCLASS()
class MANAJOURNEY_API ADragonSword : public ABasicWeapon
{
	GENERATED_BODY()

	ADragonSword();
	virtual void handleAttackEffect(float effectTime, USkeletalMeshComponent* mesh, float attackingStateToUse) override;

	// relase the impact effect of this weapon depending on the current attacking state
	void releaseImpactEffect(USkeletalMeshComponent* currentMeshToHit, float currentAttackingState);
	void destroyEffect(UParticleSystemComponent* effect2Destroy);

	//particle effects
	UParticleSystem* attackEffect;
	UParticleSystemComponent* attackEffectComp;

	//particle effects
	UParticleSystem* impactEffect1;
	UParticleSystemComponent* impactEffectComp1;

	//particle effects
	UParticleSystem* impactEffect2;
	UParticleSystemComponent* impactEffectComp2;

	//particle effects
	UParticleSystem* specialAttackEffect;
	UParticleSystemComponent* specialAttackComp;

	float attackingState = 0;

};
