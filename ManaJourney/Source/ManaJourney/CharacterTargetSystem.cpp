// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "Enemy/BasicEnemy.h"
#include "NPC/BasicNpc.h"
#include "CharacterTargetSystem.h"


ACharacterTargetSystem::ACharacterTargetSystem()
{

}

void ACharacterTargetSystem::findEnemyToTarget(ACharacter* player, UWorld* currentGameWorld, float distance)
{
	cleanUpTargets(currentGameWorld);
	findEnemyTargetByDistance(player, currentGameWorld, distance);
}

void ACharacterTargetSystem::findNPCToTarget(ACharacter* player, UWorld* currentGameWorld, float distance)
{
	cleanUpTargets(currentGameWorld);
	findNPCTargetByDistance(player, currentGameWorld, distance);
}

void ACharacterTargetSystem::findNPCTargetByDistance(ACharacter* player, UWorld* currentGameWorld, float distance)
{
	ABasicNpc* npcChar;
	float distanceToPlayer;
	float distanceToTarget = distance;
	bool npcFreeForTarget = checkFreeTargetNpcs(currentGameWorld);



	// if every enemy has been skipped, set all to be focusable again
	if (!npcFreeForTarget)
	{
		makeAllNpcsFocusable(currentGameWorld);
	}

	// get distance to enemy that can be focused
	for (TObjectIterator<ABasicNpc> npcFocusIterator; npcFocusIterator; ++npcFocusIterator)
	{
		if (npcFocusIterator->GetWorld() == currentGameWorld)
		{
			npcChar = *npcFocusIterator;
			if (npcChar && player && npcChar->bSkippedTarget == false)
			{
				distanceToPlayer = npcChar->GetDistanceTo(player);
				if (distanceToPlayer < distanceToTarget)
				{
					npcChar->bSkippedTarget = true;
					npcChar->setFocus();
					targetedNpc = npcChar;
					break;
				}
				else
				{
					npcChar->removeFocus();
					targetedNpc = nullptr;
				}
			}
			else
			{
				npcChar->removeFocus();
				targetedNpc = nullptr;
			}
		}

	}
}

bool ACharacterTargetSystem::checkFreeTargetNpcs(UWorld* currentGameWorld)
{
	ABasicNpc* npcChar;
	bool npcFreeForTarget = false;
	// check if every enemy has been focused yet
	for (TObjectIterator<ABasicNpc> npcIterator; npcIterator; ++npcIterator)
	{
		if (npcIterator->GetWorld() == currentGameWorld)
		{
			npcChar = *npcIterator;
			bool isEnemySkipped = npcChar->bSkippedTarget;
			if (!isEnemySkipped)
			{
				npcFreeForTarget = true;
			}
		}
	}

	return npcFreeForTarget;
}


void ACharacterTargetSystem::makeAllNpcsFocusable(UWorld* currentGameWorld)
{
	ABasicNpc* npcChar;
	for (TObjectIterator<ABasicNpc> npcFreeIterator; npcFreeIterator; ++npcFreeIterator)
	{
		if (npcFreeIterator->GetWorld() == currentGameWorld)
		{
			npcChar = *npcFreeIterator;
			if (npcChar)
			{
				npcChar->bSkippedTarget = false;
			}
		}
	}
}

void ACharacterTargetSystem::findEnemyTargetByDistance(ACharacter* player, UWorld* currentGameWorld, float distance)
{
	ABasicEnemy* enemyChar;
	float distanceToPlayer;
	float distanceToTarget = distance;
	//TODO CHECK THIS IN DISTANCE
	bool enemyFreeForTarget = checkFreeTargetEnemies(player, currentGameWorld, distance);

	// if every enemy has been skipped, set all to be focusable again
	if (!enemyFreeForTarget)
	{
		makeAllEnemiesFocusable(currentGameWorld);
	}

	// get distance to enemy that can be focused
	for (TObjectIterator<ABasicEnemy> enemyFocusIterator; enemyFocusIterator; ++enemyFocusIterator)
	{
		if (enemyFocusIterator->GetWorld() == currentGameWorld)
		{
			enemyChar = *enemyFocusIterator;
			if (enemyChar && !enemyChar->IsPendingKill() && player && enemyChar->bSkippedTarget == false)
			{
				distanceToPlayer = enemyChar->GetDistanceTo(player);
				if (distanceToPlayer < distanceToTarget)
				{
					enemyChar->bSkippedTarget = true;
					enemyChar->setFocus();
					targetedEnemy = enemyChar;
					break;
				}
				else
				{
					enemyChar->removeFocus();
				}
			}
			else
			{
				enemyChar->removeFocus();
				targetedEnemy = nullptr;
			}
		}

	}
}

// check only for enemies which are free to target in the given range
bool ACharacterTargetSystem::checkFreeTargetEnemies(ACharacter* player, UWorld* currentGameWorld, float distance)
{
	ABasicEnemy* enemyChar;
	bool enemyFreeForTarget = false;
	float distanceToTarget = distance;
	// TODO
	for (TObjectIterator<ABasicEnemy> enemyIterator; enemyIterator; ++enemyIterator)
	{
		if (enemyIterator->GetWorld() == currentGameWorld)
		{
			enemyChar = *enemyIterator;
			float distanceToPlayer = enemyChar->GetDistanceTo(player);
			if (!enemyChar->IsPendingKill() && distanceToPlayer < distanceToTarget)
			{
				bool isEnemySkipped = enemyChar->bSkippedTarget;
				if (!isEnemySkipped)
				{
					enemyFreeForTarget = true;
				}
			}

		}
	}
	return enemyFreeForTarget;
}

void ACharacterTargetSystem::makeAllEnemiesFocusable(UWorld* currentGameWorld)
{
	ABasicEnemy* enemyChar;
	for (TObjectIterator<ABasicEnemy> enemyFreeIterator; enemyFreeIterator; ++enemyFreeIterator)
	{
		if (enemyFreeIterator->GetWorld() == currentGameWorld)
		{
			enemyChar = *enemyFreeIterator;
			if (enemyChar && !enemyChar->IsPendingKill())
			{
				enemyChar->bSkippedTarget = false;
			}
		}
	}

}

void ACharacterTargetSystem::cleanUpTargets(UWorld* currentGameWorld)
{
	// clean up all previous focused enemies
	for (TObjectIterator<ABasicEnemy> enemyFocusIterator; enemyFocusIterator; ++enemyFocusIterator)
	{
		if (enemyFocusIterator->GetWorld() == currentGameWorld)
		{
			ABasicEnemy* enemyChar = *enemyFocusIterator;
			if (!enemyChar->IsPendingKill())
			{
				enemyChar->hideFocusComponent();
				enemyChar->removeFocus();
				enemyChar->disableMenu();
			}
		}
	}

	// clean up all previous focused enemies
	for (TObjectIterator<ABasicNpc> npcFocusIterator; npcFocusIterator; ++npcFocusIterator)
	{
		if (npcFocusIterator->GetWorld() == currentGameWorld)
		{
			ABasicNpc* npcChar = *npcFocusIterator;
			npcChar->hideFocusComponent();
			npcChar->removeFocus();
		}
	}

}