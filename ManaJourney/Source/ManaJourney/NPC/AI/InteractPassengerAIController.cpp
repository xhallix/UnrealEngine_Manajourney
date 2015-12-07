// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Name.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "../BasicPassenger.h"
#include "InteractPassengerAIController.h"



void AInteractPassengerAIController::initBlackboardComponents(UBlackboardComponent* blackboardComponent)
{

	if (blackboardComponent)
	{
		FString blackboardKeyID_ShouldDoAnimation = "ShouldDoAnimation";
		blackboardComponent->SetValue<UBlackboardKeyType_Bool>(blackboardComponent->GetKeyID(FName(*blackboardKeyID_ShouldDoAnimation)), true);
		APawn* controlledPawn = GetPawn();
		ABasicPassenger* passenger = Cast<ABasicPassenger>(controlledPawn);

		//@TODO Divide in seperate functions
		if (passenger)
		{
			//Sets animationSequences from the current passenger to be used in animation tree
			TArray<FName> passengerAnimationSequence = passenger->animationSequence;
			FName animSequence;
			for (int32 i = 0; i < passengerAnimationSequence.Num(); i++)
			{
				animSequence = passengerAnimationSequence[i];
				FString blackboardKeyAnimID = "AnimationSequence";
				blackboardKeyAnimID.AppendInt(i);
				blackboardComponent->SetValue<UBlackboardKeyType_Name>(blackboardComponent->GetKeyID(FName(*blackboardKeyAnimID)), animSequence);
			}


			//Sets animation waiting time from the current passenger to be used in animation tree

			TArray<float> passengerAnimationDelay = passenger->delayForAnimation;
			float animDelay;
			for (int32 x = 0; x < passengerAnimationDelay.Num(); x++)
			{
				animDelay = passengerAnimationDelay[x];
				FString blackboardKeyDelayID = "AnimationDelay";
				blackboardKeyDelayID.AppendInt(x);
				blackboardComponent->SetValue<UBlackboardKeyType_Float>(blackboardComponent->GetKeyID(FName(*blackboardKeyDelayID)), animDelay);
			}


			// checks wether we should initialize a static or moving AI character
			bool shouldFollowTargetPoints = passenger->shouldFollowTargetPoints;
			FString blackboardKeyID_ShouldFollowTargetPoints = "ShouldFollowTargetPoints";
			blackboardComponent->SetValue<UBlackboardKeyType_Bool>(blackboardComponent->GetKeyID(FName(*blackboardKeyID_ShouldFollowTargetPoints)), shouldFollowTargetPoints);

			// when using moving character invoke super class method
			if (shouldFollowTargetPoints)
			{
				Super::setTargetPointsAsBlackboardComponent(blackboardComponent);
			}
		}

	}



}
