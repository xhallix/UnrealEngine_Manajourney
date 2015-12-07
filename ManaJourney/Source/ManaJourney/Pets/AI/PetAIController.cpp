// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "../BasicPet.h"
#include "../../Enemy/BasicEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "PetAIController.h"


void APetAIController::BeginPlay()
{
	APawn* controlledPawn = GetPawn();
	if (controlledPawn)
	{
		petToControl = Cast<ABasicPet>(controlledPawn);
		if (petToControl)
		{
			petToControl->AssignOwner(nullptr);
		}
		this->BBComponentsInitialized();
	}
}


void APetAIController::initBlackboardComponents(UBlackboardComponent* blackboardComponent)
{
	if (blackboardComponent)
	{
		// set blackboardKey - bHasAttackingMode
		AIblackboardComponent = blackboardComponent;
		setAttackingMode(AIblackboardComponent, false);
	}

}

void APetAIController::setAttackingState()
{
	ABasicPet::ATTACKING_STATE++;
	if (ABasicPet::ATTACKING_STATE > 2.0f)
	{
		ABasicPet::ATTACKING_STATE = 1.0f;
	}
	setAnimationTimeDelay();
}

//TODO this is only valid for the Troll, othe characters needs different delays
void APetAIController::setAnimationTimeDelay()
{
	if (AIblackboardComponent)
	{
		float delay = 1.0f;
		if (ABasicPet::ATTACKING_STATE == 1.0f){ delay = 1.9f;}
		if (ABasicPet::ATTACKING_STATE == 2.0f){ delay = 1.3f; }
		AIblackboardComponent->SetValue<UBlackboardKeyType_Float>(AIblackboardComponent->GetKeyID("animationTimeDelay"), delay);

	}
}

void APetAIController::setAttackingMode(UBlackboardComponent* blackboardComponent, bool bStatus)
{
	if (blackboardComponent)
	{
		// set blackboardKey - bHasAttackingMode
		FString blackboardKeyID_bHasAttackingMode = "bHasAttackingMode";
		blackboardComponent->SetValue<UBlackboardKeyType_Bool>(blackboardComponent->GetKeyID(FName(*blackboardKeyID_bHasAttackingMode)), bStatus);
	}
}

bool APetAIController::IsOwnerNear(float radius)
{
	float acceptableRadius = (radius) ? petToControl->distanceAcceptToOwner : radius;
	bool isOwnerNear = false;
	APawn* controlledPawn = GetPawn();
	FVector petLocation = controlledPawn->GetActorLocation();
	ACharacter* petOwner = petToControl->GetPetOwner();
	if (petOwner)
	{
		FVector petOwnerLocation = petOwner->GetActorLocation();
		isOwnerNear = petLocation.Equals(petOwnerLocation, acceptableRadius);	
	}

	if (isOwnerNear)
	{
		petToControl->StopWalking();
	}
	
	return isOwnerNear;
}


bool APetAIController::IsEnemyNear(float radius)
{
	float acceptableRadius = (radius) ? petToControl->distanceAcceptToEnemy : radius;
	bool isEnemyNear = false;
	FVector petLocation = GetPawn()->GetActorLocation();
	ACharacter* petOwner = petToControl->GetPetOwner();
	AManaJourneyCharacter* manaJourneyCharacter = Cast<AManaJourneyCharacter>(petOwner);
	ABasicEnemy* petEnemy;

	// check if enemyIsNear
	if (manaJourneyCharacter && manaJourneyCharacter->enemyFound != nullptr && !manaJourneyCharacter->enemyFound->bIsDead)
	{
		petEnemy = Cast<ABasicEnemy>(manaJourneyCharacter->enemyFound);
		if (petEnemy)
		{
			FVector enemyLocation = petEnemy->GetActorLocation();
			isEnemyNear = petLocation.Equals(enemyLocation, 200.0f);
			//perform attack when near
			if (isEnemyNear)
			{
				petToControl->StopWalking();
				setAttackingState();
				petToControl->InitAttack(petEnemy);
			}
		}
	}

	// enemy is not present, dont look at him!
	else
	{
		petToControl->StopMeleeAttack();
		petToControl->StopWalking();
		isEnemyNear = false;
	}

	return isEnemyNear;

}

void APetAIController::MoveToOwner()
{
	petToControl->StopMeleeAttack();
	petToControl->ReleaseEnemy();
	petToControl->StartWalking(300.0f);
	ACharacter* petOwner = petToControl->GetPetOwner();
	MoveToActor(petOwner, petToControl->distanceAcceptToOwner);
}

void APetAIController::MoveToEnemy()
{
	petToControl->StartWalking(300.0f);
	ACharacter* petOwner = petToControl->GetPetOwner();
	AManaJourneyCharacter* manaJourneyCharacter = Cast<AManaJourneyCharacter>(petOwner);
	ABasicEnemy* petEnemy;

	if (manaJourneyCharacter)
	{
		if (manaJourneyCharacter->enemyFound != nullptr)
		{
			petEnemy = Cast<ABasicEnemy>(manaJourneyCharacter->enemyFound);
			if (petEnemy)
			{
				MoveToActor(petEnemy, petToControl->distanceAcceptToEnemy);
			}
		}
	}

}


void APetAIController::DetermineAttackingMode()
{
	if (petToControl->ShouldAttack())
	{
		setAttackingMode(AIblackboardComponent, true);
	}
	else
	{
		setAttackingMode(AIblackboardComponent, false);
	}
	
}
