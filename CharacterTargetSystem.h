// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManaJourneyCharacter.h"
#include "Enemy/BasicEnemy.h"
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

	void findTargetByDistance(ACharacter* player, UWorld* currentGameWorld);

	void findEnemyToTarget(ACharacter* player, UWorld* currentGameWorld);

	ABasicEnemy* targetedEnemy;
	
};
