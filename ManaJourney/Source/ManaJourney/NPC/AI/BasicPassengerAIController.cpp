// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "../BasicPassenger.h"
#include "BasicPassengerAIController.h"

ABasicPassengerAIController::ABasicPassengerAIController()
{
	
}


void ABasicPassengerAIController::setTargetPointsAsBlackboardComponent(UBlackboardComponent* blackboardComponent)
{
	APawn* controlledPawn = GetPawn();
	ABasicPassenger* passenger = Cast<ABasicPassenger>(controlledPawn);

	if (passenger && blackboardComponent)
	{
		TArray<FVector> passengerTargetPoints = passenger->targetPoints;
		FVector targetPoint;
		for (int32 i = 0; i < passengerTargetPoints.Num(); i++)
		{
			targetPoint = passengerTargetPoints[i];
			FString blackboardKeyID = "Target";
			blackboardKeyID.AppendInt(i);
			FBlackboard::FKey blackBoardTargetKey = blackboardComponent->GetKeyID(FName(*blackboardKeyID));

			blackboardComponent->SetValue<UBlackboardKeyType_Vector>(blackBoardTargetKey, targetPoint);
		}
	}

}

bool ABasicPassengerAIController::hasPawnReachedLocation(FVector vectorToReach, float acceptedRadius)
{
	bool pawnReachedLocation = false;
	float radiusToCheck;
	APawn* controlledPawn = GetPawn();
	ABasicPassenger* currentPassenger = Cast<ABasicPassenger>(controlledPawn);
	if (currentPassenger && !acceptedRadius || acceptedRadius == 0.0f)
	{
		radiusToCheck = currentPassenger->acceptableRadius;
	}
	else
	{
		radiusToCheck = acceptedRadius;
	}
	FVector pawnLocation = controlledPawn->GetActorLocation();
	pawnReachedLocation = pawnLocation.Equals(vectorToReach, radiusToCheck);
	return pawnReachedLocation;
}

