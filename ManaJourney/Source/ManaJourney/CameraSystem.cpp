// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "NPC/BasicNpc.h"
#include "CameraSystem.h"

ACameraSystem::ACameraSystem()
{

}

void ACameraSystem::enableNpcCamera(AManaJourneyCharacter* cameraOwner, ABasicNpc* cameraTarget)
{
	cameraOwner->disableWalkAnimation();
	cameraOwner->toggleCharacterMovement();
	cameraOwner->FollowCamera->Deactivate();
	cameraOwner->DialogCamera->SetActive(true);
	cameraOwner->alternativeCameraActive = true;
	cameraOwner->faceNpc(cameraTarget);
	cameraTarget->enableDialogSystem();
}

void ACameraSystem::disableNpcCamera(AManaJourneyCharacter* cameraOwner, ABasicNpc* cameraTarget)
{
	cameraOwner->enableWalkAnimation();
	cameraOwner->GetCharacterMovement()->Activate();
	cameraOwner->FollowCamera->SetActive(true);
	cameraOwner->DialogCamera->Deactivate();
	cameraOwner->alternativeCameraActive = false;
	cameraTarget->disableDialogSystem();
}
