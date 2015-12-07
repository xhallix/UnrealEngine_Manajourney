// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManaJourneyCharacter.h"
#include "Enemy/BasicEnemy.h"
#include "NPC/BasicNpc.h"
#include "CharacterTargetSystem.generated.h"

/**
 * 
 */
UCLASS()
class MANAJOURNEY_API ACharacterTargetSystem : public AManaJourneyCharacter
{
	GENERATED_BODY()
	
public:
	ACharacterTargetSystem();

	void findEnemyToTarget(ACharacter* player, UWorld* currentGameWorld, float distance);
	void findEnemyTargetByDistance(ACharacter* player, UWorld* currentGameWorld, float distance);
	// check if there is an enemy which can be focused
	bool checkFreeTargetEnemies(ACharacter* player, UWorld* currentGameWorld, float distance);
	// makes every enemy in the gameworld focusable
	void makeAllEnemiesFocusable(UWorld* currentGameWorld);


	void findNPCToTarget(ACharacter* player, UWorld* currentGameWorld, float distance);
	void findNPCTargetByDistance(ACharacter* player, UWorld* currentGameWorld, float distance);
	

	bool checkFreeTargetNpcs(UWorld* currentGameWorld);
	void makeAllNpcsFocusable(UWorld* currentGameWorld);


	// removes every focus from the given class
	void cleanUpTargets(UWorld* currentGameWorld);

	ABasicEnemy* targetedEnemy;
	ABasicNpc* targetedNpc;
	
};
