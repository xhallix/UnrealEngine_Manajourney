// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ManaJourney.h"
#include "GameFramework/DamageType.h"
#include "CharacterTargetSystem.h"
#include "CameraSystem.h"
#include "Enemy/BasicEnemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "ManaClasses/WhiteMage.h"
#include "ManaClasses/BlackMage.h"
#include "ManaClasses/Summoner.h"
#include "Weapon/Staff.h"
#include "Weapon/DragonSword.h"
#include "DamageTypes/MeleeDamageType.h"
#include "DamageTypes/ManaDamageType.h"
#include "Combat/MeleeCombatSystem.h"
#include "ManaJourneyCharacter.h"



//////////////////////////////////////////////////////////////////////////
// AManaJourneyCharacter

AManaJourneyCharacter::AManaJourneyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	characterMovementComp = GetCharacterMovement();
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DialogCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DialogCamera"));
	DialogCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	DialogCamera->bUsePawnControlRotation = false;

	// location can be adjusted depending on the weapon
	MeeleCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeeleCapsule"));
	MeeleCapsule->AttachParent = GetMesh();

	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	meleeCombatSystem = nullptr;
}

// Called every frame
void AManaJourneyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!enemyFound && npcFound && player)
	{
		faceNpc(npcFound);
	}
}

void AManaJourneyCharacter::faceNpc(ABasicNpc* npcFound)
{
	if (npcFound)
	{
		const FVector playerLocation = player->GetActorLocation();
		const FVector npcLocation = npcFound->GetActorLocation();
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(playerLocation, npcLocation);
		SetActorRotation(FMath::Lerp(GetActorRotation(), lookRotation, 0.2f));
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AManaJourneyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAxis("MoveForward", this, &AManaJourneyCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AManaJourneyCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AManaJourneyCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AManaJourneyCharacter::LookUpAtRate);

	InputComponent->BindAction("TargetEnemy", IE_Pressed, this, &AManaJourneyCharacter::enableTargetSystem);
	InputComponent->BindAction("ReleaseEnemy", IE_Pressed, this, &AManaJourneyCharacter::disableTargetSystem);
	InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &AManaJourneyCharacter::meleeAttack);

	/* WHITEMAGE*/
	InputComponent->BindAction("useCure", IE_Pressed, this, &AManaJourneyCharacter::useCure);
	InputComponent->BindAction("useInvisible", IE_Pressed, this, &AManaJourneyCharacter::useInvisible);
	InputComponent->BindAction("useShell", IE_Pressed, this, &AManaJourneyCharacter::useShell);
	
	/* BLACKMAGE */
	InputComponent->BindAction("useFire", IE_Pressed, this, &AManaJourneyCharacter::useFire);
	InputComponent->BindAction("useTornado", IE_Pressed, this, &AManaJourneyCharacter::useTornado);
	InputComponent->BindAction("useBind", IE_Pressed, this, &AManaJourneyCharacter::useBind);


	InputComponent->BindAction("interact", IE_Pressed, this, &AManaJourneyCharacter::interaction);
}

void AManaJourneyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AManaJourneyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AManaJourneyCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AManaJourneyCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


AManaJourneyCharacter* AManaJourneyCharacter::GetSelfReference()
{
	return this;
}

void AManaJourneyCharacter::levelUp()
{
	// updates based on base attributes
	level = level + 1;
	health = 10.0f + level * 1.5;
	maxHealth = health;
	mana = 20 +  level * 1.5;
	strength = 10.0f + level * 1.5;
	nextExp = nextExp + 100.0f + level * 2;

	if (blackMageClassRef)
	{
		blackMageClassRef->levelUp(level);
		
	}

	if (whiteMageClassRef)
	{
		whiteMageClassRef->levelUp(level);
	}

	if (summonerClassRef)
	{

	}
}


void AManaJourneyCharacter::gainExp(float gainedExp)
{
	currentExp = currentExp + gainedExp;
	if (currentExp >= nextExp)
	{
		currentExp = 0.0f;
		levelUp();
	}
}

void AManaJourneyCharacter::enableTargetSystem()
{
	// currentGame world is needed for safe object iteration when using TObjectIterator
	UWorld* currentGameWorld = GetWorld();
	// get class from CharacterTargetSystem
	ACharacterTargetSystem* targetSystem = Cast<ACharacterTargetSystem>(ACharacterTargetSystem::StaticClass()->GetDefaultObject());
	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (targetSystem)
	{
		targetSystem->findEnemyToTarget(player, currentGameWorld, 1000.0f);
		if (targetSystem->targetedEnemy)
		{
			enemyFound = targetSystem->targetedEnemy;
		}
		if (enemyFound && enemyFound != nullptr && enemyFound->bInitialised)
		{
			enemyFound->showFocusComponent();
			if (!weaponEquiped)
			{
				toggleCharacterMovement();
				this->prepareForBattle();
				weaponEquiped = true;
				// activate character movement
				FTimerHandle WeaponHandler;
				GetWorldTimerManager().ClearTimer(WeaponHandler);
				FTimerDelegate WeaponHandlerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCharacterMovement);
				GetWorldTimerManager().SetTimer(WeaponHandler, WeaponHandlerDelegate, 1.00f, false);
			}

		}
		// it is not possible to find an enemy && npc, prefer enemy selection over npc (but both should not be in the same lookup area
		else
		{
			targetSystem->findNPCToTarget(player, currentGameWorld, 500.0f);
			npcFound = targetSystem->targetedNpc;
			if (npcFound)
			{
				npcFound->showFocusComponent();
				enableNpcCamera();
			}
		}

	}
}

void AManaJourneyCharacter::disableTargetSystem()
{

	// only perfom disabling when enemy if found, for perfomance sake
	if (weaponEquiped)
	{
		GetCharacterMovement()->Deactivate();
		this->unprepareForBattle();
		weaponEquiped = false;

		// active charater movement
		FTimerHandle WeaponHandler;
		GetWorldTimerManager().ClearTimer(WeaponHandler);
		GetWorldTimerManager().SetTimer(WeaponHandler, FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::activateCharacterMovement), 1.00f, false);
	}

	ACharacterTargetSystem* targetSystem = Cast<ACharacterTargetSystem>(ACharacterTargetSystem::StaticClass()->GetDefaultObject());
	if (targetSystem)
	{
		targetSystem->cleanUpTargets(GetWorld());
	}

	if (npcFound)
	{
		disableNpcCamera(npcFound);
	}

	// check if we found an enemy
	npcFound = nullptr;
	enemyFound = nullptr;
}


void AManaJourneyCharacter::activateCharacterMovement()
{
	GetCharacterMovement()->Activate();
}

void AManaJourneyCharacter::useMeleeCombatSystem(AMeleeCombatSystem* meleeCombatSystemToUse)
{
	if (meleeCombatSystemToUse)
	{
		meleeCombatSystem = meleeCombatSystemToUse;
		meleeCombatSystem->setUserReference(this);
	} 
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No meleeCombatSystem provided for ManaJourneyCharacter")));
	}
}


void AManaJourneyCharacter::useWhiteMage(AWhiteMage* whiteMageClass)
{
	if (whiteMageClass)
	{
		FString whiteMage = "whiteMageClass";
		characterClasses.Add(whiteMage);
		whiteMageClassRef = whiteMageClass;
		whiteMageClass->referenceCharacterMovement = characterMovementComp;
	}
}


void AManaJourneyCharacter::useBlackMage(ABlackMage* blackMageClass)
{
	if (blackMageClass)
	{
		FString blackMage = "blackMageClass";
		characterClasses.Add(blackMage);
		blackMageClassRef = blackMageClass;
		blackMageClass->referenceCharacterMovement = characterMovementComp;
	}
}

// updates only physic protection (include reduce mana)
void AManaJourneyCharacter::updatePhysicProtection()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Character has shell %i"), whiteMageClassRef->bHasShell));
	if (whiteMageClassRef->bHasShell)
	{
		fadeTimeForEffect = whiteMageClassRef->fShellTimer + level;
		physicProtection = physicProtection + level * 1.5;
	} 
	else
	{
		physicProtection = physicProtection - level * 1.5;
		toggleCastOngoing();
	}

}

void AManaJourneyCharacter::receiveDamage(float amount)
{
	if (physicProtection > 1)
	{
		amount = (amount / physicProtection);
	}
	health = (health - amount);
}

void AManaJourneyCharacter::reduceMana(float amount)
{
	mana = mana - amount;
}

void AManaJourneyCharacter::useCure()
{
	// use whitemage class and power depending on the level

	if (characterClasses.Contains("whiteMageClass") && !castOngoing)
	{
		toggleCastOngoing();
		bool bCastedCure = false;
		bCastedCure = whiteMageClassRef->castCure(*this, GetMesh());
		if (bCastedCure)
		{
			castHealAnimation(level);
		}
		// timer for setting cast to false
		FTimerHandle CastOngoingHandler;
		GetWorldTimerManager().ClearTimer(CastOngoingHandler);
		FTimerDelegate CastOngoingHandlerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCastOngoing);
		GetWorldTimerManager().SetTimer(CastOngoingHandler, CastOngoingHandlerDelegate, 1.00f, false);
	}

	// use summoner class
}


void AManaJourneyCharacter::useInvisible()
{
	if (characterClasses.Contains("whiteMageClass") && whiteMageClassRef && !castOngoing)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Character cast invisible")));
		toggleCastOngoing();
		float manaCosts = level * 1.5;
		if (mana > manaCosts)
		{
			toggleCharacterMovement();
			castInvisibleAnimation(level);
		}
		// timer for particle effect destruction
		FTimerHandle InvisibleCast;
		GetWorldTimerManager().ClearTimer(InvisibleCast);
		FTimerDelegate invisibleCastDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::beginCastInvisible);
		GetWorldTimerManager().SetTimer(InvisibleCast, invisibleCastDelegate, 1.00f, false);

		// timer for setting cast to false
		FTimerHandle CastOngoingHandler;
		GetWorldTimerManager().ClearTimer(CastOngoingHandler);
		FTimerDelegate CastOngoingHandlerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCastOngoing);
		GetWorldTimerManager().SetTimer(CastOngoingHandler, CastOngoingHandlerDelegate, 7.00f, false);
	}
}

void AManaJourneyCharacter::useShell()
{
	if (characterClasses.Contains("whiteMageClass") && whiteMageClassRef && !castOngoing)
	{
		toggleCastOngoing();
		bool bCastedShell = false;
		bCastedShell = whiteMageClassRef->castShell(*this, GetMesh());
		if (bCastedShell)
		{
			toggleCharacterMovement();
			updatePhysicProtection();
			castShellAnimation(level);
			toggleCastOngoing();

			//NOTE remove shell effect after fadeTimeForEffect has set to false (so add slight delay for beeing sure) 
			FTimerHandle UpdatePhysicProtectionTimer;
			GetWorldTimerManager().ClearTimer(UpdatePhysicProtectionTimer);
			FTimerDelegate UpdatePhysicProtectionTimerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::updatePhysicProtection);
			GetWorldTimerManager().SetTimer(UpdatePhysicProtectionTimer, UpdatePhysicProtectionTimerDelegate, fadeTimeForEffect + 0.2f, false);

			FTimerHandle EnableMovementTimer;
			GetWorldTimerManager().ClearTimer(EnableMovementTimer);
			FTimerDelegate updateCharacterMovementDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCharacterMovement);
			GetWorldTimerManager().SetTimer(EnableMovementTimer, updateCharacterMovementDelegate, 1.50f, false);

			// timer for setting cast to false
			FTimerHandle CastOngoingHandler;
			GetWorldTimerManager().ClearTimer(CastOngoingHandler);
			FTimerDelegate CastOngoingHandlerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCastOngoing);
			GetWorldTimerManager().SetTimer(CastOngoingHandler, CastOngoingHandlerDelegate, whiteMageClassRef->fShellTimer, false);
		}
	}
}

void AManaJourneyCharacter::useFire()
{
	if (characterClasses.Contains("blackMageClass") && !castOngoing && enemyFound)
	{
		toggleCastOngoing();
		animationTimer = 3.0f;
		bool bCastedFire = false;
		bCastedFire = blackMageClassRef->castFire(*this, GetMesh());
		if (bCastedFire)
		{
			castFireAnimation(level);
		}
		// timer for setting cast to false
		FTimerHandle CastOngoingHandler;
		GetWorldTimerManager().ClearTimer(CastOngoingHandler);
		FTimerDelegate CastOngoingHandlerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCastOngoing);
		GetWorldTimerManager().SetTimer(CastOngoingHandler, CastOngoingHandlerDelegate, animationTimer, false);
		
	}
}

void AManaJourneyCharacter::useTornado()
{
	if (characterClasses.Contains("blackMageClass") && !castOngoing && enemyFound)
	{
		toggleCastOngoing();
		animationTimer = 3.0f;
		bool bCasted = false;
		bCasted = blackMageClassRef->castTornado(*this, GetMesh());
		if (bCasted)
		{
			castTornadoAnimation(level);
		}
		// timer for setting cast to false
		FTimerHandle CastOngoingHandler;
		GetWorldTimerManager().ClearTimer(CastOngoingHandler);
		FTimerDelegate CastOngoingHandlerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCastOngoing);
		GetWorldTimerManager().SetTimer(CastOngoingHandler, CastOngoingHandlerDelegate, animationTimer, false);

	}
}

void AManaJourneyCharacter::useBind()
{
	if (characterClasses.Contains("blackMageClass") && !castOngoing && enemyFound)
	{
		toggleCastOngoing();
		animationTimer = 3.0f;
		bool bCasted = false;
		bCasted = blackMageClassRef->castBind(*this, GetMesh());
		if (bCasted)
		{
			castBindAnimation(level);
		}
		// timer for setting cast to false
		FTimerHandle CastOngoingHandler;
		GetWorldTimerManager().ClearTimer(CastOngoingHandler);
		FTimerDelegate CastOngoingHandlerDelegate = FTimerDelegate::CreateUObject(this, &AManaJourneyCharacter::toggleCastOngoing);
		GetWorldTimerManager().SetTimer(CastOngoingHandler, CastOngoingHandlerDelegate, animationTimer, false);
	}
}

void AManaJourneyCharacter::equipItems(FName SocketName, TArray<AStaticMeshActor*> equipments)
{
	AActor* equipActor;
	FString equipmentName;

	for (int32 e = 0; e < equipments.Num(); e++)
	{
		equipActor = equipments[e];
		if (equipActor)
		{
			equipmentName = equipActor->GetName();

			// move this to seperate function

			if (equipmentName.Contains("NightwingStaff"))
			{
				equipItem(equipActor, "Staff");
				AStaff* staff = Cast<AStaff>(equipActor);
				if (staff)
				{
					staff->setUserStrength(level, *this);
					staff->setWeaponDamage(*this);
					weaponReference = staff;
				}
			}

			// NOTE: The BP the weapon is made of, MUST be based on the corresponding c++ class ( e.g DragonSword c++ class )
			if (equipmentName.Contains("DragonSword"))
			{
				equipItem(equipActor, "Sword_1");
				ADragonSword* dragonSword = Cast<ADragonSword>(equipActor);
				if (dragonSword)
				{
					dragonSword->setUserStrength(level, *this);
					dragonSword->setWeaponDamage(*this);
					weaponReference = dragonSword;
				}
			}

		}
	}

}



void AManaJourneyCharacter::equipItem(AActor* actorToEquip, FName SocketName)
{

	// set default properties or the equipment in Editor
	if (actorToEquip)
	{
		actorToEquip->SetActorEnableCollision(false);
		actorToEquip->GetRootComponent()->SetMobility(EComponentMobility::Movable);
		actorToEquip->AttachRootComponentTo(this->GetMesh(), SocketName, EAttachLocation::SnapToTargetIncludingScale, false);

		// remove any custom depth for equipped items
		TArray<UStaticMeshComponent*> StaticMeshesList;
		actorToEquip->GetComponents<UStaticMeshComponent>(StaticMeshesList);

		for (int32 i = 0; i < StaticMeshesList.Num(); i++)
		{
			UStaticMeshComponent* StaticMeshComponent = StaticMeshesList[i];
			StaticMeshComponent->SetRenderCustomDepth(false);
		}
	}
	
}


void AManaJourneyCharacter::toggleCharacterMovement()
{
	if (GetCharacterMovement()->IsActive())
	{
		GetCharacterMovement()->Deactivate();
	}
	else
	{
		GetCharacterMovement()->Activate();
	}

}

void AManaJourneyCharacter::beginCastInvisible()
{
	toggleCharacterMovement();
	whiteMageClassRef->castInvisible(*this, GetMesh());
}
void AManaJourneyCharacter::toggleCastOngoing()
{
	if (castOngoing)
	{
		castOngoing = false;
	}
	else
	{
		castOngoing = true;
	}
}

void AManaJourneyCharacter::enableNpcCamera()
{
	ACameraSystem* cameraSystem = Cast<ACameraSystem>(ACameraSystem::StaticClass()->GetDefaultObject());
	if (cameraSystem)
	{
		cameraSystem->enableNpcCamera(this, npcFound);
	}
}

void AManaJourneyCharacter::disableNpcCamera(ABasicNpc* npcFound)
{
	ACameraSystem* cameraSystem = Cast<ACameraSystem>(ACameraSystem::StaticClass()->GetDefaultObject());
	if (cameraSystem)
	{
		cameraSystem->disableNpcCamera(this, npcFound);
	}
}


void AManaJourneyCharacter::interaction()
{

	if (npcFound && npcFound->isDialogSystemEnabled)
	{
		npcFound->nextDialog();
	}

}

// TODO should be a template function to 
ABasicWeapon* AManaJourneyCharacter::getWeaponReference()
{
	return weaponReference;
}


void AManaJourneyCharacter::meleeAttack()
{
	if (weaponEquiped && meleeCombatSystem)
	{
		meleeCombatSystem->init();
	}

	if (!meleeCombatSystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("No meleeCombatSystem provided for ManaJourneyCharacter:::Cannot attack")));
	}
}


void AManaJourneyCharacter::TakeCharacterDamage(float Damage, UDamageType* damageType, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (!bIsDead)
	{
		health = health - Damage;
		if (health <= 0.0f)
		{
			handleDeath();
		}
		else
		{
			handleHitByDamageType(DamageCauser, damageType);
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("you are dead")));
	}

}
void AManaJourneyCharacter::handleHitByDamageType(AActor* DamageCauser, UDamageType* damageType)
{
	UManaDamageType* manaDamageType;
	// use the timer provided by the damage type
	manaDamageType = Cast<UManaDamageType>(damageType);
	if (manaDamageType)
	{
		//TODO
	}
	else
	{
		handleHit();
		// on melee attacks we use push back (currently this push back is not depending on melee damage type, but this is a possible implementation)
		//pushBackByDamageCauser(100000.0f, DamageCauser);
	}

}

void AManaJourneyCharacter::handleHit()
{
	this->receiveHitAnimation();
}


void AManaJourneyCharacter::handleDeath()
{
	// TODO
}