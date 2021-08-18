// Copyright Epic Games, Inc. All Rights Reserved.

#include "KidRedGameGameMode.h"
#include "KidRedGameHUD.h"
#include "KidRedGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKidRedGameGameMode::AKidRedGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AKidRedGameHUD::StaticClass();
}
