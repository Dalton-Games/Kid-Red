// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "GameFramework/PlayerController.h"
#include "KidRedPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KIDREDCORE_API AKidRedPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKidRedPlayerController();

	UFUNCTION(BlueprintCallable, Exec, Category = "HUD")
	void ShowHUD();

	UFUNCTION(BlueprintCallable, Exec, Category = "HUD")
	void HideHUD();
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ToggleHUD(bool bShowHUD);

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Client, Reliable, Category = "Pawn")
			void Client_OnPossess(APawn* InPawn);
	virtual void Client_OnPossess_Implementation(APawn* InPawn);
};
