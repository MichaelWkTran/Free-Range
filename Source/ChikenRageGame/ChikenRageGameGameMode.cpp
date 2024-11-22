// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChikenRageGameGameMode.h"
#include "ChikenRageGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AChikenRageGameGameMode::AChikenRageGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/PlayerChicken/BP_PlayerChicken"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//Set Gameplay Parameters
	CheckpointNum = 6;
}
