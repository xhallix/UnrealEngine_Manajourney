// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "../BasicEnemy.h"
#include "../../Pets/BasicPet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BasicEnemyAIController.h"

void ABasicEnemyAIController::BeginPlay()
{
	this->BBComponentsInitialized();
}


void ABasicEnemyAIController::FindAggressorPet()
{
	for (TObjectIterator<ABasicPet> petIterator; petIterator; ++petIterator)
	{
		if (petIterator->GetWorld() == GetWorld() && !petIterator->bIsDead)
		{
			aggressorPet = *petIterator;
		}

	}
}

void ABasicEnemyAIController::initBlackboardComponents(UBlackboardComponent* blackboardComponent)
{
	if (blackboardComponent)
	{
		AIblackboardComponent = blackboardComponent;
		setAttackingMode(AIblackboardComponent, false);
		setTargetPointsAsBlackboardComponent(blackboardComponent);
	}

}

void ABasicEnemyAIController::setTargetPointsAsBlackboardComponent(UBlackboardComponent* blackboardComponent)
{
	APawn* controlledPawn = GetPawn();
	ABasicEnemy* basicEnemy = Cast<ABasicEnemy>(controlledPawn);

	if (basicEnemy && blackboardComponent)
	{
		TArray<FVector> targetPoints = basicEnemy->targetPoints;
		FVector targetPoint;
		for (int32 i = 0; i < targetPoints.Num(); i++)
		{
			targetPoint = targetPoints[i];
			FString blackboardKeyID = "Target";
			blackboardKeyID.AppendInt(i);
			FBlackboard::FKey blackBoardTargetKey = blackboardComponent->GetKeyID(FName(*blackboardKeyID));

			blackboardComponent->SetValue<UBlackboardKeyType_Vector>(blackBoardTargetKey, targetPoint);
		}
	}

}

void ABasicEnemyAIController::setAttackingState()
{
	ABasicEnemy::ATTACKING_STATE++;
	if (ABasicEnemy::ATTACKING_STATE > 2.0f)
	{
		ABasicEnemy::ATTACKING_STATE = 1.0f;
	}

}

void ABasicEnemyAIController::setAnimationTimeDelay(ABasicEnemy* basicEnemy,bool shouldClear)
{
	if (AIblackboardComponent)
	{
		float delay = 1.0f;
		TArray <float> animationDelays = basicEnemy->animationDelays;
		if (ABasicEnemy::ATTACKING_STATE == 1.0f){ delay = (animationDelays.Num() > 1) ? animationDelays[0] : 1.9f; }
		if (ABasicEnemy::ATTACKING_STATE == 2.0f){ delay = (animationDelays.Num() > 2) ? animationDelays[1] : 1.3f; }
		AIblackboardComponent->SetValue<UBlackboardKeyType_Float>(AIblackboardComponent->GetKeyID("animationTimeDelay"), delay);

		if (shouldClear)
		{
			AIblackboardComponent->SetValue<UBlackboardKeyType_Float>(AIblackboardComponent->GetKeyID("animationTimeDelay"), 0.2f);
		}
	}


}


void ABasicEnemyAIController::setAttackingMode(UBlackboardComponent* blackboardComponent, bool bStatus)
{
	if (blackboardComponent)
	{
		// set blackboardKey - bHasAttackingMode
		FString blackboardKeyID_bHasAttackingMode = "bHasAttackingMode";
		blackboardComponent->SetValue<UBlackboardKeyType_Bool>(blackboardComponent->GetKeyID(FName(*blackboardKeyID_bHasAttackingMode)), bStatus);
	}
}

// defines the aggressor
bool ABasicEnemyAIController::IsEnemyNear(float radius)
{
	bool isEnemyNear = false;
	bool isPet = false;
	ABasicEnemy* controlledEnemy = Cast<ABasicEnemy>(GetPawn());
	ACharacter* aggressorPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (controlledEnemy) 
	{
		FVector playerLocation = aggressorPlayer->GetActorLocation();
		FVector enemyActorLocation = GetPawn()->GetActorLocation();
		float acceptableRadius = (radius) ? radius : controlledEnemy->aggroRange;
		isEnemyNear = enemyActorLocation.Equals(playerLocation, acceptableRadius);
	
		// todo maybe move to seperate function
		if (!isEnemyNear)
		{
			FindAggressorPet();
			if (aggressorPet != nullptr && !aggressorPet->bIsDead)
			{
				isEnemyNear = enemyActorLocation.Equals(aggressorPet->GetActorLocation(), acceptableRadius);
				isPet = (isEnemyNear) ? true : false;
			} 
		}

		// should attack
		if (isEnemyNear)
		{
			if (isPet && aggressorPet)
			{
				aggressor = (!aggressorPet->bIsDead) ? Cast<AActor>(aggressorPet) : nullptr;
			}
			else
			{
				aggressor = Cast<AActor>(aggressorPlayer);
			}
			setAttackingMode(AIblackboardComponent, true);
			setAttackingState();
			setAnimationTimeDelay(controlledEnemy);
		}
		else
		{
			setAttackingMode(AIblackboardComponent, false);
			setAnimationTimeDelay(controlledEnemy, true);
		}
	}

	return isEnemyNear;
}

// invoke melee attack means running and attacking
void ABasicEnemyAIController::InvokeMeleeAttack()
{
	// if is near attack otherwise, run to it
	ABasicEnemy* controlledEnemy = Cast<ABasicEnemy>(GetPawn());
	
	if (controlledEnemy)
	{
		float rangeToAttack = controlledEnemy->attackRadius;
		bool isEnemyNear = IsEnemyNear(rangeToAttack);
		if (isEnemyNear)
		{
			controlledEnemy->InitMeleeAttack();
		}
		else
		{
			setAnimationTimeDelay(controlledEnemy, true);
			MoveToEnemy(controlledEnemy);
		}
	} 
	
}


void ABasicEnemyAIController::MoveToEnemy(ABasicEnemy* controlledEnemy)
{
	if (aggressor && aggressor != nullptr)
	{
		controlledEnemy->StopAttacking();
		controlledEnemy->StartWalking(controlledEnemy->followSpeed);
		controlledEnemy->attacker = aggressor;
		MoveToActor(aggressor, controlledEnemy->attackRadius);
	}
}

void ABasicEnemyAIController::StopMeleeAttack()
{
	ABasicEnemy* controlledEnemy = Cast<ABasicEnemy>(GetPawn());

	if (controlledEnemy)
	{
		controlledEnemy->StopAttacking();
	}
}

void ABasicEnemyAIController::StopMoveTo()
{
	ABasicEnemy* controlledEnemy = Cast<ABasicEnemy>(GetPawn());
	FVector pawnLocation = GetPawn()->GetActorLocation();

	if (controlledEnemy)
	{
		MoveToActor(controlledEnemy, controlledEnemy->attackRadius);
	}
}

bool ABasicEnemyAIController::hasPawnReachedLocation(FVector vectorToReach, float acceptedRadius)
{
	bool pawnReachedLocation = false;
	float radiusToCheck;
	APawn* controlledPawn = GetPawn();
	ABasicEnemy* ctrlPawn = Cast<ABasicEnemy>(controlledPawn);
	if (ctrlPawn && !acceptedRadius || acceptedRadius == 0.0f)
	{
		radiusToCheck = ctrlPawn->acceptablePatrolRadius;
	}
	else
	{
		radiusToCheck = acceptedRadius;
	}
	FVector pawnLocation = controlledPawn->GetActorLocation();
	pawnReachedLocation = pawnLocation.Equals(vectorToReach, radiusToCheck);
	return pawnReachedLocation;
}

bool ABasicEnemyAIController::CanMoveToTargetPoint(FVector vectorToReach, float acceptedRadius)
{
	ABasicEnemy* controlledEnemy = Cast<ABasicEnemy>(GetPawn());
	bool canMoveToTargetPoint = true;
	if (controlledEnemy)
	{
		bool isEnemyNear = IsEnemyNear(controlledEnemy->aggroRange);
		canMoveToTargetPoint = (!isEnemyNear) ? hasPawnReachedLocation(vectorToReach, 0.0f) : isEnemyNear;
	}
	return canMoveToTargetPoint;
}