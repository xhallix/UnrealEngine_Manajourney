/*
*	BasicPassenger just walks around in towns without any special interaction 
*
*/

#pragma once

#include "GameFramework/Character.h"
#include "BasicPassenger.generated.h"

UCLASS()
class MANAJOURNEY_API ABasicPassenger : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ABasicPassenger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Target points the passenger will pass during gameplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		TArray<FVector>targetPoints;

	// Target points the passenger will pass during gameplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		bool shouldFollowTargetPoints = false;

	// Indicates the radius for setted target points. If the passenger should walk in small distance it is recommended to lowe this
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		float acceptableRadius = 50.0f;

	// animation sequence the passenger will play during gameplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		TArray<FName>animationSequence;

	// time between animation sequences the passenger will wait during gameplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIAttributes")
		TArray<float>delayForAnimation;
};
