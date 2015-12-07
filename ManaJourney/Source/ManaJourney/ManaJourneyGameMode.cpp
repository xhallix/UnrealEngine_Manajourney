// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ManaJourney.h"
#include "ManaJourneyGameMode.h"
#include "ManaJourneyCharacter.h"

AManaJourneyGameMode::AManaJourneyGameMode()
{
	// set default pawn class to our Blueprinted characterGame/Enemies/TestEnemy.TestEnemy
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/Aeris"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
