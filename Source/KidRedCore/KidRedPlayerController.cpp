// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedPlayerController.h"

AKidRedPlayerController::AKidRedPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.0f;
	
	bFindCameraComponentWhenViewTarget = true;

	SetCanBeDamaged(false);
}

void AKidRedPlayerController::ShowHUD()
{
	ToggleHUD(true);
}

void AKidRedPlayerController::HideHUD()
{
	ToggleHUD(false);
}

void AKidRedPlayerController::ToggleHUD(const bool bShowHUD)
{
	if (AHUD* HUD = GetHUD())
	{
		HUD->bShowHUD = bShowHUD;
	}
}

void AKidRedPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Client_OnPossess(InPawn);
}

void AKidRedPlayerController::Client_OnPossess_Implementation(APawn* InPawn)
{
	if (AKidRedHUD* HUD = GetHUD<AKidRedHUD>())
	{
		HUD->InitializeHUD();
	}
}
