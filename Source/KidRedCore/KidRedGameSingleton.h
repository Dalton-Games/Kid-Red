// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "KidRedGameSingleton.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class KIDREDCORE_API UKidRedGameSingleton final : public UObject
{
	GENERATED_BODY()

public:
	static UKidRedGameSingleton& Get();

	UFUNCTION(BlueprintPure, Category = "Game")
	static UKidRedGameSingleton* GetKidRedGameSingleton();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Tables")
	const UDataTable* AnimationTable = nullptr;
};
