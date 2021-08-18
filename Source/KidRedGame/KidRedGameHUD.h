// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KidRedGameHUD.generated.h"

UCLASS()
class AKidRedGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AKidRedGameHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

