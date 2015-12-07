// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "BasicPassenger.h"


// Sets default values
ABasicPassenger::ABasicPassenger()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicPassenger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicPassenger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ABasicPassenger::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

