// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ManaJourneyCharacter.h"
#include "NPC/BasicNpc.h"
#include "CameraSystem.generated.h"

/**
 * 
 */
UCLASS()
class MANAJOURNEY_API ACameraSystem : public AManaJourneyCharacter
{
	GENERATED_BODY()
	
	
public:
	ACameraSystem();

	void enableNpcCamera(AManaJourneyCharacter* cameraOwner, ABasicNpc* cameraTarget);
	void disableNpcCamera(AManaJourneyCharacter* cameraOwner, ABasicNpc* cameraTarget);
	
};
