// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

UCLASS()
class MANAJOURNEY_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicEnemy(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;



public:

	static float ATTACKING_STATE;

	UPROPERTY(Category = Private, VisibleDefaultsOnly)
		UStaticMeshComponent* FocusComponent;

	UPROPERTY(Category = Private, VisibleDefaultsOnly, BlueprintReadOnly)
		UTextRenderComponent* DamageTextComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float exp = 1.00f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		FString enemyName = "basicEnemy";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float strength = 1.00f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float mana = 1.00f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float maxMana = mana;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float enemyHealth = 1.00f;

	// Radius whichin a search is performed to check if the enemy is near
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		float aggroRange = 200.0f;

	// indicates how far away he will stand during attack
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		float attackRadius = 140.0f;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float enemyMaxHealth = enemyHealth;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		FString elementProne = "Fire";

	// speed to follow the aggressor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		float followSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		bool bIsDead = false;

	// Delay for attackingstates (how long the animation will take)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		TArray<float> animationDelays;

	// Target points the passenger will pass during gameplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		bool shouldFollowTargetPoints = false;

	// Target points the passenger will pass during gameplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		TArray<FVector>targetPoints;

	// custom class for takeDamage native implementation
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void TakeCharacterDamage(float Damage, UDamageType* damageType, class AController* EventInstigator, class AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ReceiveHitAnimation"), Category = "Reaction")
		void receiveHitAnimation();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterComponents")
		UCapsuleComponent* MeleeCapsule;

	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
		TSubclassOf<UDamageType> MeleeDamage;

	// enemy menu
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "EnableMenu"), Category = "Gameplay")
		void enableMenu();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "DisableMenu"), Category = "Gameplay")
		void disableMenu();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StartWalking"), Category = "Gameplay")
		void StartWalking(float speed);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StopWalking"), Category = "Gameplay")
		void StopWalking();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MeleeAttackBP"), Category = "Attack")
		void meleeAttackBP(float damageAmount, float attackState);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StopAttacking"), Category = "Gameplay")
		void StopAttacking();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MoveDamageText"), Category = "Gameplay")
		void moveDamageText(FVector vector2Move, FRotator rotatorToMove, FVector initialVector, FRotator initialRotator, FVector initialDamageTextScale);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UseAttackEffect"), Category = "Attack")
		void UseAttackEffect(AActor* attacker);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PlayDeathEffect"), Category = "Attack")
		void PlayDeathEffect();

public:

	// focused by target system
	bool bFocused = false;
	// indicates if this enemy has been focused, so it can be skipped to select the next
	bool bSkippedTarget = false;
	

	// sets bFocused
	void toggleFocus();
	void setFocus();
	void removeFocus();
	void showFocusComponent();
	void hideFocusComponent();

	void handleDeath(AActor* DamageCauser);
	void handleDeathByDamageType(AActor* DamageCauser, UDamageType* damageType);
	void handleHitByDamageType(AActor* DamageCauser, UDamageType* damageType);
	void handleHit();

	void InitMeleeAttack();
	void AttackManaChar();
	void AttackPet();
	void FaceAttacker();
	void DamageEnemy(AActor* enemyToAttack);
	AActor* attacker;


	void destroyEffect(UParticleSystemComponent* effect2Destroy);
	// visualizes the taken damage 
	void handleDamageText(float damageAmount, AActor* DamageCauser);
	void hideDamageTextComponent();

	void pushBackByDamageCauser(float pushBackPower, AActor* DamageCauser);

	FVector initialDamageTextLocation = FVector(-30, 75, 120);
	FVector initialDamageTextScale = FVector(0, 0, 0);
	FRotator initialDamageTextRotation = FRotator(0, 90, 00);
	// true when constructor called fully
	bool bInitialised = false;


	float acceptablePatrolRadius = 50.0f;





};
