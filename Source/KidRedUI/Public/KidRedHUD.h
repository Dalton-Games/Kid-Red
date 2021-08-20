// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "GameFramework/HUD.h"
#include "KidRedHUD.generated.h"

UCLASS()
class KIDREDUI_API AKidRedHUD : public AHUD
{
	GENERATED_BODY()

public:
	AKidRedHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void InitializeHUD(bool bForce = false);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	uint8 bEnableHUD : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (EditCondition = "bEnableHUD"))
	TSubclassOf<class UKidRedHUDWidget> HUD_Class = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UKidRedHUDWidget* PlayerHUDWidget = nullptr;

private:
	void InitializeGameStatsOverlay();
	
	TSharedPtr<class SGameStatsOverlay> GameStatsOverlayWidget;
	TSharedPtr<class SWidget> GameStatsOverlayContainer;
};
