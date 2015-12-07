// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "Enemy/BasicEnemy.h"
#include "NPC/BasicNpc.h"
#include "ManaJourneyCharacter.generated.h"

//Forward declaration to avoid circular dependencies
class ABlackMage;
class ABasicWeapon;
class ASummoner;
class ACameraSystem;
class AMeleeCombatSystem;

UCLASS(config=Game)
class AManaJourneyCharacter : public ACharacter
{
	GENERATED_BODY()


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** DialogCamera  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* DialogCamera;

	friend class ACameraSystem;
	
public:
	AManaJourneyCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// actions for spells
	void useCure();
	void useInvisible();
	void useShell();
	void useFire();
	void useTornado();
	void useBind();

	void meleeAttack();


	// general interaction in gameplay like handling dialogs, open doors etc
	void interaction();


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// character attributes 
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float mana = 20.0f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float maxMana = mana;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float health = 10.0f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float maxHealth = health;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float strength = 10.0f;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float level = 1.0f;

	// protection agains physical attacks
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float physicProtection = 2.0f;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float currentExp = 1.00f;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterAttributes")
		float nextExp = 50.00f;

	// indicates the time which each melee attack animation needs to complete. This will be used by the combat system to ensure we do not interrupt animations
	// the timer needs to be the same as in the blueprint 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterAttributes")
		TArray<float>meleeAttackSequences;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterComponents")
		UCapsuleComponent* MeeleCapsule;

	// checks if a cast is possible, it is not if another cast is ongoing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CharacterComponents")
		bool castOngoing = false;

	// changes or add the manaClass of the character
	UFUNCTION(BlueprintCallable, Category = "MeleeCombat")
		void useMeleeCombatSystem(AMeleeCombatSystem* meleeCombatSystemToUse);

	// changes or add the manaClass of the character
	UFUNCTION(BlueprintCallable, Category = "MageClass")
		void useWhiteMage(AWhiteMage* whiteMageClass);
	
	// changes or add the manaClass of the character
	UFUNCTION(BlueprintCallable, Category = "MageClass")
		void useBlackMage(ABlackMage* blackMageClass);

	// all logic when beeing hit
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void receiveDamage(float amount);

	// used when casting
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void reduceMana(float amount);

	// equip the character with the given item
	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void equipItems(FName SocketName, TArray<AStaticMeshActor*> equipments);

	UFUNCTION(BlueprintCallable, Category = "Equipment")
		void equipItem(AActor* actorToEquip, FName SocketName);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void toggleCastOngoing();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void levelUp();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void gainExp(float gainedExp);

	// custom class for takeDamage native implementation
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void TakeCharacterDamage(float Damage, UDamageType* damageType, class AController* EventInstigator, class AActor* DamageCauser);

	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CastHeal"), Category = "Spells")
		void castHealAnimation(float levelForAnimation);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CastInvisible"), Category = "Spells")
		void castInvisibleAnimation(float levelForAnimation);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CastShell"), Category = "Spells")
		void castShellAnimation(float levelForAnimation);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CastFire"), Category = "Spells")
		void castFireAnimation(float levelForAnimation);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CastTornado"), Category = "Spells")
		void castTornadoAnimation(float levelForAnimation);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "CastBind"), Category = "Spells")
		void castBindAnimation(float levelForAnimation);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "SelectedWhiteMage"), Category = "Gameplay")
		void selectedWhiteMage();

	// enable animation and socket snapping when a character is ready to attack an enemy
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PrepareForBattle"), Category = "Gameplay")
		void prepareForBattle();

	// enable animation and socket snapping when a character is not targeting an enemy
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UnprepareForBattle"), Category = "Gameplay")
		void unprepareForBattle();

	// enable animation for attack
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "MeleeAttackBP"), Category = "Attack")
		void meleeAttackBP(float damageAmount, float attackState);

	// event to stop walk animation in BP
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "disableWalkAnimation"), Category = "Movement")
		void disableWalkAnimation();

	// event to stop walk animation in BP
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "enableWalkAnimation"), Category = "Movement")
		void enableWalkAnimation();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ReceiveHitAnimation"), Category = "Reaction")
		void receiveHitAnimation();

	UPROPERTY(EditDefaultsOnly, Category = "DamageType")
		TSubclassOf<UDamageType> MeleeDamage;

public:

	TArray <FString> characterClasses;
	UCharacterMovementComponent* characterMovementComp;
	AWhiteMage* whiteMageClassRef;
	ABlackMage* blackMageClassRef;
	ASummoner* summonerClassRef;
	AMeleeCombatSystem* meleeCombatSystem;
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ABasicWeapon* weaponReference;
	APlayerController* playerController;
	ABasicEnemy* enemyFound;
	ABasicNpc* npcFound;

	AManaJourneyCharacter* GetSelfReference();

	// set all values for physic protection
	void updatePhysicProtection();
	void toggleCharacterMovement();
	void activateCharacterMovement();


	// beginCastInvisible needs a delay before executing for making the animation 
	void beginCastInvisible();
	void enableTargetSystem();
	void disableTargetSystem();
	void faceNpc(ABasicNpc* npcFound);
	void disableNpcCamera(ABasicNpc* npcFound);
	void enableNpcCamera();
	
	ABasicWeapon* getWeaponReference();
	bool weaponEquiped = false;


	void handleHitByDamageType(AActor* DamageCauser, UDamageType* damageType);
	void handleHit();
	void handleDeath();
	bool bIsDead = false;



	// indicates when an effect, which the character has (like shell etc will fade)
	float fadeTimeForEffect = 1.00f;
	// time that indicates when the current animation has been finished for Casting
	float animationTimer = 0.00f;
	bool alternativeCameraActive = false;

};


