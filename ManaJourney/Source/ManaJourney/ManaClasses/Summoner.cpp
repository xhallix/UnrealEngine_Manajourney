// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "Summoner.h"


// Sets default values
ASummoner::ASummoner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASummoner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASummoner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

