// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../ManaJourneyCharacter.h"
#include "MeleeCombatSystem.generated.h"





/****************	MELEE LOGIC	**********/
/*
*	@Melee Logic
*	This is a simple melee combat system where we can chain up to three different animations
*	Attacks will only be chained if we use one attack quickly after another
*	USAGE:
*	Create a BP based on this and give assign it to the character which will use this.
*	Assignment will be made by using AManaJourneyCharacter->useMeleeCombatSystem() which is BPcallable
*	The character needs to be AManaJourneyCharacter because it has certain needed abilities like strange etc
*
*	Schema is:
*	
*	1. updateAttack state
*	2. block attackstate
*	3. invokeMeleeAttack
*	4. release attack state
*
*	@function setUserReference - Set the user for this system, needs to be called first
*	@function init - Initialising this system
*	@function invokeAttackingStates - Opens timeframe to start next attack
*	where a new attacking state can be set
*	@function invokeMeleeCombat - Entry point for logic
*	@function updateAttackingState - counts up the attacking states after each attack
*	@freeAttackingState - Enable to start nex attacking states
*	@clearAttackingState - clears the attacking state after 2 seconds
*	@setAnimationTimeFrame - Set the timeframe how long each animation in the current state is
*	@bool isMeleeAttackOngoing - Attack can be performed
*	@bool canStartNextAttackingState - When true and melee attack started the next attackingstate is used
*/
UCLASS()
class MANAJOURNEY_API AMeleeCombatSystem : public AActor
{
	GENERATED_BODY()
	

private:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// seconds count for deltatime
	float seconds = 0.0f;

public: 
	

	void setUserReference(AManaJourneyCharacter* characterToUse);
	void init();
	void invokeAttackingStates();
	void invokeMeleeCombat(float damageAmount);
	void updateAttackingState();
	void clearAttackingState();
	void canStartNewMeleeAttack();
	void handleWeaponEffects(float time);
	void damageTargetedEnemy();
	void blockAttackingState();
	void freeAttackingState();
	void setAnimationTimeFrame();
	void setTimeFrames();

	bool isMeleeAttackOngoing = false;
	bool canStartNextAttackingState = true;
	bool shoulResetAttackingStates = false;

	// indicates how often the melee attack has been performed (refering to melee states)
	float currentAttackState = 0.0f;
	float storedAttackState = 0.0f;
	// timerFrames to proceed with next action
	float timeFrameForAttackingState = 0.2f;
	float timeFrameToNewMeleeAttack = 0.2f;

	AManaJourneyCharacter* characterReference;
	TArray <float>animationFrames;




};
