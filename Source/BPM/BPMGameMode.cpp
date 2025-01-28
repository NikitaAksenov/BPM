// Copyright Epic Games, Inc. All Rights Reserved.

#include "BPMGameMode.h"
#include "BPMCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABPMGameMode::ABPMGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
