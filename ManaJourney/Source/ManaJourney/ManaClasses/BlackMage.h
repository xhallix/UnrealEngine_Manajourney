// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../ManaJourneyCharacter.h"
#include "BlackMage.generated.h"

UCLASS()
class MANAJOURNEY_API ABlackMage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackMage();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
		TSubclassOf<UDamageType> FireDamage;

	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
		TSubclassOf<UDamageType> TornadoDamage;

	bool castFire(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh);
	bool castBind(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh);
	bool castTornado(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh);

	void castSpellEffect(AManaJourneyCharacter* character, USkeletalMeshComponent* enemyMesh, UParticleSystemComponent* particleComponent, UParticleSystem* particleSystem, float destroyTimer);
	void destroyEffect(UParticleSystemComponent* effect2Destroy);
	void castInvokerEffect(AManaJourneyCharacter* character);
	void toggleCharacterMovement(UCharacterMovementComponent* characterMovementComponent);

	// use this after level up to change effects of the spells
	void initFireSpells(float level);
	void initAirSpells(float level);
	void levelUp(float level);


	// allows to handle character movement on cure
	UCharacterMovementComponent* referenceCharacterMovement;



	/*
	* Current implementation holds 2 particle effects for each mana cast
	* 1. particleEffect is delay destroyed -> assign to delayFireParticle
	* 2. particleEffect is auto destroyed -> assign to autoFireParticle
	* Assigned in the corresponding handle function like initFireSpells
	* CommonParticleComponent used to delay destroy
	* 
	*/

	//particle effects
	UParticleSystem* fire1particleEffect;
	UParticleSystem* fire2particleEffect;
	UParticleSystem* fire1particleEffect2;
	UParticleSystem* fire2particleEffect2;
	// commons
	UParticleSystemComponent* commonFireParticleComponent;
	UParticleSystem* delayFireParticle;
	UParticleSystem* autoFireParticle;

	UParticleSystem* tornado1particleEffect;
	UParticleSystem* tornado2particleEffect;
	UParticleSystem* tornado1particleEffect2;
	UParticleSystem* tornado2particleEffect2;

	// commons
	UParticleSystemComponent* commonTornadoParticleComponent;
	UParticleSystem* delayTornadoParticle;
	UParticleSystem* autoTornadoParticle;

	UParticleSystem* bind1particleEffect;
	UParticleSystemComponent* bind1particleEffectComp;

	// effects which are sourrounding the character
	UParticleSystem* particleEffectCharacter;
	UParticleSystemComponent* particleEffectCompCharacter;
};
