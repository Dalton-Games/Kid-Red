// Copyright Epic Games, Inc. All Rights Reserved.

#include "KidRedHUD.h"

#include "SGameStatsOverlay.h"

AKidRedHUD::AKidRedHUD()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = TICK_RATE_30;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	bEnableHUD = true;
}

void AKidRedHUD::InitializeHUD(const bool bForce)
{
	if (!bEnableHUD)
		return;
	
	if (PlayerHUDWidget && !bForce)
		return;

	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->RemoveFromParent();
		PlayerHUDWidget = nullptr;
	}

	PlayerHUDWidget = CreateWidget<UKidRedHUDWidget>(GetWorld(), HUD_Class);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
		PlayerHUDWidget->SetVisibility(bShowHUD ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void AKidRedHUD::BeginPlay()
{
	Super::BeginPlay();

	InitializeGameStatsOverlay();
}

void AKidRedHUD::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bEnableHUD)
		return;
	
	if (PlayerHUDWidget)
	{
		// Don't keep setting visibility every frame, just do it once
		if (bShowHUD)
		{
			if (!PlayerHUDWidget->IsVisible())
				PlayerHUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			if (PlayerHUDWidget->IsVisible())
				PlayerHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void AKidRedHUD::InitializeGameStatsOverlay()
{
	if (GameStatsOverlayWidget.Get())
		return;
	
	if (GetWorld())
	{
		if (GetWorld()->GetGameViewport())
		{
			GameStatsOverlayWidget = SNew(SGameStatsOverlay).OwningHUD(this);
			GetWorld()->GetGameViewport()->AddViewportWidgetContent(SAssignNew(GameStatsOverlayContainer, SWeakWidget).PossiblyNullContent(GameStatsOverlayWidget.ToSharedRef()));
		}
	}
}