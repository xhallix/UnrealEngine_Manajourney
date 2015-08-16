// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "Enemy/BasicEnemy.h"
#include "CharacterTargetSystem.h"


ACharacterTargetSystem::ACharacterTargetSystem()
{
	
}

void ACharacterTargetSystem::findEnemyToTarget(ACharacter* player, UWorld* currentGameWorld)
{
	findTargetByDistance(player, currentGameWorld);
}

void ACharacterTargetSystem::findTargetByDistance(ACharacter* player, UWorld* currentGameWorld)
{
	ABasicEnemy* enemyChar;
	float distanceToPlayer;
	float distanceToTarget = 1000.00f;
	bool enemyFreeForTarget = false;

	// TODO!!! BETTER USE TOBJECTITERATOR ONCE AND PUSH IT TO A GLOABL ARRAY IN MANACHAR

	// check if one enemy has not been skipped yet
	for (TObjectIterator<ABasicEnemy> enemyIterator; enemyIterator; ++enemyIterator)
	{
		if (enemyIterator->GetWorld() == currentGameWorld)
		{
			enemyChar = *enemyIterator;
			bool isEnemySkipped = enemyChar->bSkippedTarget;
			if (!isEnemySkipped)
			{
				enemyFreeForTarget = true;
			}
		}
	}

	// if every enemy has been skipped, set all to be focusable again
	if (!enemyFreeForTarget)
	{
		for (TObjectIterator<ABasicEnemy> enemyFreeIterator; enemyFreeIterator; ++enemyFreeIterator)
		{
			if (enemyFreeIterator->GetWorld() == currentGameWorld)
			{
				enemyChar = *enemyFreeIterator;
				if (enemyChar)
				{
					enemyChar->bSkippedTarget = false;
				}
			}
			
		}
	}

	// get distance to enemy that can be focused
	for (TObjectIterator<ABasicEnemy> enemyFocusIterator; enemyFocusIterator; ++enemyFocusIterator)
	{
		if (enemyFocusIterator->GetWorld() == currentGameWorld)
		{
			enemyChar = *enemyFocusIterator;
			//		if (player && enemyChar && enemyChar->bSkippedTarget == false)
			if (enemyChar && player && enemyChar->bSkippedTarget == false)
			{
				distanceToPlayer = enemyChar->GetDistanceTo(player);
				if (distanceToPlayer < distanceToTarget)
				{
					enemyChar->bSkippedTarget = true;
					enemyChar->toggleFocus();
					targetedEnemy = enemyChar;
					break;
				}
			}
		}

	}
}


