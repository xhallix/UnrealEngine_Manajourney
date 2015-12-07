// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WhiteMage.generated.h"

UCLASS()
class MANAJOURNEY_API AWhiteMage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWhiteMage();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


public:
	// Class attributes

	// use as a main or subclass for character
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ClassAttributes")
		bool bisMainClass;

	
	// Helper functions
	UFUNCTION(BlueprintCallable, Category = "ClassHelper")
		AActor* getActor();


public:

	// Class spells
	bool castCure(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh);
	void castCureEffect(USkeletalMeshComponent* mesh);
	bool castInvisible(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh);
	void castInvisibleEffect(USkeletalMeshComponent* mesh);
	void toggleVisibility(USkeletalMeshComponent* mesh);

	bool castShell(AManaJourneyCharacter& character, USkeletalMeshComponent* mesh);
	void castShellEffect(USkeletalMeshComponent* mesh);
	void setShellFalse();
	void destroyEffect(UParticleSystemComponent* effect2Destroy);
	UStaticMesh* equipClass();

	// indicate how long shell will last
	float fShellTimer = 30.0f;
	bool bHasShell = false;

	void initCureSpells(float level);
	void initShellSpells(float level);
	void levelUp(float level);

	/*
	* Current implementation holds 2 particle effects for each mana cast
	* Assigned in the corresponding handle function like initShellSpells
	* Every particle effect is delay destroyed here
	* CommonParticleComponent used to delay destroy
	*
	*/

	// add new effect here
	UParticleSystem* cure1particleEffect;
	UParticleSystem* cure2particleEffect;
	UParticleSystem* cure1particleEffect2;
	UParticleSystem* cure2particleEffect2;

	// commons and delay can be reused
	UParticleSystemComponent* commonCureParticleComponent1;
	UParticleSystemComponent* commonCureParticleComponent2;
	UParticleSystem* delayCureParticle1;
	UParticleSystem* delayCureParticle2;

	// add new effect here
	UParticleSystem* shell1particleEffect;
	UParticleSystem* shell2particleEffect;
	UParticleSystem* shell1particleEffect2;
	UParticleSystem* shell2particleEffect2;

	// commons and delay can be reused
	UParticleSystemComponent* commonShellParticleComponent1;
	UParticleSystemComponent* commonShellParticleComponent2;
	UParticleSystem* delayShellParticle1;
	UParticleSystem* delayShellParticle2;



	UParticleSystem* invisibleparticleEffect;
	UParticleSystemComponent* invisibleparticleEffectComp;

	// allows to handle character movement on cure
	UCharacterMovementComponent* referenceCharacterMovement;

};
