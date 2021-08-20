// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedGameModeBase.h"
#include "KidRedCharacter.h"
#include "KidRedUI/Public/KidRedHUD.h"
#include "KidRedPlayerController.h"
#include "KidRedPlayerState.h"
#include "KidRedGameStateBase.h"
#include "KidRedSpectatorPawn.h"
#include "KidRedGameSession.h"

AKidRedGameModeBase::AKidRedGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.0f;
	
	DefaultPawnClass = AKidRedCharacter::StaticClass();
	HUDClass = AKidRedHUD::StaticClass();
	PlayerControllerClass = AKidRedPlayerController::StaticClass();
	GameStateClass = AKidRedGameStateBase::StaticClass();
	PlayerStateClass = AKidRedPlayerState::StaticClass();
	SpectatorClass = AKidRedSpectatorPawn::StaticClass();
	GameSessionClass = AKidRedGameSession::StaticClass();
}
