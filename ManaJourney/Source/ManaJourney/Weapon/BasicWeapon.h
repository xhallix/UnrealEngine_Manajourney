// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../ManaJourneyCharacter.h"
#include "BasicWeapon.generated.h"

UCLASS()
class MANAJOURNEY_API ABasicWeapon : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ItemAttributes")
		float strength = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
		TSubclassOf<UDamageType> WeaponDamage;
	
	void setUserStrength(float level, AManaJourneyCharacter& character);
	void setWeaponDamage(AManaJourneyCharacter& character);
	
	// used to be overwritten by child class
	virtual void handleAttackEffect(float effectTime, USkeletalMeshComponent* mesh, float attackingStateToUse);
	void destroyEffect(UParticleSystemComponent* effect2Destroy);
	
	// indicates the mesh where the particle effect shall be attached
	USkeletalMeshComponent* meshToHit;

	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterComponents")
		UCapsuleComponent* MeeleCapsule;
	
};
