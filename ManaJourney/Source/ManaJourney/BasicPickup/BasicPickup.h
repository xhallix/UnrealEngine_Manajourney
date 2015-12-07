/*
*	This actor is a simple interactable item which can be picked up 
*
*/

#pragma once

#include "GameFramework/Actor.h"
#include "BasicPickup.generated.h"

UCLASS()
class MANAJOURNEY_API ABasicPickup : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "CharacterComponents")
		UBoxComponent* TriggerBox;
	
};
