// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "PetAIController.generated.h"

/**
 * 
 */
UCLASS()
class MANAJOURNEY_API APetAIController : public AAIController
{
	GENERATED_BODY()

private:
	ABasicPet* petToControl;
	UBlackboardComponent* AIblackboardComponent;
	
public:
	

	void BeginPlay() override;
	void setAttackingMode(UBlackboardComponent* blackboardComponent, bool bStatus = false);
	void setAttackingState();
	void setAnimationTimeDelay();
	
	
	// initializes the blackboardcomponent keys for this aicontroller
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void initBlackboardComponents(UBlackboardComponent* blackboardComponent);

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		bool IsOwnerNear(float radius);

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		bool IsEnemyNear(float radius);
	
	
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void MoveToOwner();

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void MoveToEnemy();

	
	// set the blackboardkey to true if enemy is targeted
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void DetermineAttackingMode();


	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BBComponentsInitialized"), Category = "AIUtils")
		void BBComponentsInitialized();


};
