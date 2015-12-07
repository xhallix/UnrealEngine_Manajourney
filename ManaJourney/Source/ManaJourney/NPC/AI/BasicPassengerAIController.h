// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BasicPassengerAIController.generated.h"

/**
 *  This AI can simple walk around
 *
 *  Use the controller to handle different condition check as blueprint callable function
 *  the ai controller can be casted in every decorator -> PerformConditionCheck -> ownerActor
 *  
 *	NOTE: The aicontroller for basicpassenger can only work if the first target point == location of the pawn in the map
 */
UCLASS()
class MANAJOURNEY_API ABasicPassengerAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABasicPassengerAIController();

	// sets all target points for the controlled passenger to the blackboard component
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void setTargetPointsAsBlackboardComponent(UBlackboardComponent* blackboardComponent);

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		bool hasPawnReachedLocation(FVector vectorToReach, float acceptedRadius);

};
