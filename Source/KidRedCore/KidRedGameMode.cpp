// Copyright Epic Games, Inc. All Rights Reserved.

#include "KidRedGameMode.h"
#include "KidRedHUD.h"
#include "KidRedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKidRedGameMode::AKidRedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AKidRedHUD::StaticClass();
}
