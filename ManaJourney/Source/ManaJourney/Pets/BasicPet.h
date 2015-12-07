// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "../Enemy/BasicEnemy.h"
#include "../ManaJourneyCharacter.h"
#include "BasicPet.generated.h"

/**
 * 
 */
UCLASS()
class MANAJOURNEY_API ABasicPet : public ACharacter
{
	GENERATED_BODY()


private:
	ACharacter* petOwner;
	ABasicEnemy* focusedEnemy;
	//particle effects
	UParticleSystem* attackEffect;
	UParticleSystemComponent* attackEffectComp;

public:
	ABasicPet();

	static float ATTACKING_STATE;


	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// character attributes 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float mana = 20.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float maxMana = mana;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float health = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float maxHealth = health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float strength = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float level = 1.0f;

	// protection agains physical attacks
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float physicProtection = 2.0f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float currentExp = 1.00f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float nextExp = 50.00f;

	// acceptable radius when moving or getting owner 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		float distanceAcceptToEnemy = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		float distanceAcceptToOwner = 400.0f;


	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
		TSubclassOf<UDamageType> MeleeDamage;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterComponents")
		UCapsuleComponent* MeleeCapsule;

	// custom class for takeDamage native implementation
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void TakeCharacterDamage(float Damage, UDamageType* damageType, class AController* EventInstigator, class AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void Ressurection();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StartWalking"), Category = "Gameplay")
		void StartWalking(float speed);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StopWalking"), Category = "Gameplay")
		void StopWalking();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StopMeleeAttack"), Category = "Gameplay")
		void StopMeleeAttack();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MeleeAttackBP"), Category = "Attack")
		void meleeAttackBP(float damageAmount, float attackState);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ReceiveHitAnimation"), Category = "Reaction")
		void receiveHitAnimation();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PlayDeathEffect"), Category = "Attack")
		void PlayDeathEffect();

	// set the owner for the pet, for reference
	void AssignOwner(ACharacter* owningActor);
	bool ShouldAttack();
	void InitAttack(ABasicEnemy* enemyToAttack);
	void DamageTargetedEnemy(ABasicEnemy* enemyToAttack);
	void InvokeMeleeAttackEffect(USkeletalMeshComponent* meshToAttach, float attackingState);
	void DestroyEffect(UParticleSystemComponent* effect2Destroy);
	void FaceEnemy();
	void ReleaseEnemy();
	ACharacter* ABasicPet::GetPetOwner();

	void handleHitByDamageType(AActor* DamageCauser, UDamageType* damageType);
	void handleHit();
	void handleDeath();
	bool bIsDead = false;
};
