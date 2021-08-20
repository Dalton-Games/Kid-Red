// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "KidRedFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class KIDREDCORE_API UKidRedFunctionLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Kid Red Function Library|World")
	static UWorld* GetWorldStatic(bool bRequiresGameState = true);
	
	UFUNCTION(BlueprintPure, Category = "Kid Red Function Library|Ease")
	static float EaseAlpha(float InAlpha, TEnumAsByte<EEasingFunc::Type> EasingFunc, float BlendExp = 2.0f, int32 Steps = 2);

	UFUNCTION(BlueprintPure, Category = "Kid Red Function Library|Ease")
	static float EaseClamped(float A, float B, float Alpha, TEnumAsByte<EEasingFunc::Type> EasingFunc, float BlendExp = 2.0f, int32 Steps = 2);
};
