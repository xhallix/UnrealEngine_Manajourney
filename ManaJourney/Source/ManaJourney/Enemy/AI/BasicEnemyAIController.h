// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "../../Pets/BasicPet.h"
#include "BasicEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MANAJOURNEY_API ABasicEnemyAIController : public AAIController
{
	GENERATED_BODY()

private:
	UBlackboardComponent* AIblackboardComponent;
	
	// Called every frame
	//virtual void Tick(float DeltaSeconds) override;

public:
		
	void BeginPlay() override;
	void setAttackingMode(UBlackboardComponent* blackboardComponent, bool bStatus = false);
	void setAttackingState();
	void setAnimationTimeDelay(ABasicEnemy* basicEnemy, bool shouldClear = false);
	void FindAggressorPet();

	// currently in range
	AActor* aggressor;
	ABasicPet* aggressorPet;

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void initBlackboardComponents(UBlackboardComponent* blackboardComponent);

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		bool IsEnemyNear(float radius);

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		bool CanMoveToTargetPoint(FVector vectorToReach, float acceptedRadius);


	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void MoveToEnemy(ABasicEnemy* controlledEnemy=nullptr);

	// set the blackboardkey to true if enemy is targeted
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void InvokeMeleeAttack();

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void StopMeleeAttack();
	
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void StopMoveTo();

	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "BBComponentsInitialized"), Category = "AIUtils")
		void BBComponentsInitialized();

	// sets all target points for the controlled passenger to the blackboard component
	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		void setTargetPointsAsBlackboardComponent(UBlackboardComponent* blackboardComponent);

	UFUNCTION(BlueprintCallable, Category = "AIUtils")
		bool hasPawnReachedLocation(FVector vectorToReach, float acceptedRadius);


};
