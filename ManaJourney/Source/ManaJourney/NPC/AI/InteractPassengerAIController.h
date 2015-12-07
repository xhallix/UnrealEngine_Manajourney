// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NPC/AI/BasicPassengerAIController.h"
#include "InteractPassengerAIController.generated.h"

/**
 *  This AI controller allows animation interaction on a certain point
 *	
 *	Use the controller to handle different condition check as blueprint callable function
 *  the ai controller can be casted in every decorator -> PerformConditionCheck -> ownerActor * 
 */
UCLASS()
class MANAJOURNEY_API AInteractPassengerAIController : public ABasicPassengerAIController
{
	GENERATED_BODY()
	
public:
	// initializes the blackboardcomponent keys for this aicontroller
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void initBlackboardComponents(UBlackboardComponent* blackboardComponent);
	
	
};
