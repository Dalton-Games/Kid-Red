// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "KidRedGameplayStatics.generated.h"

/**
 * 
 */
UCLASS()
class KIDREDCORE_API UKidRedGameplayStatics final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Returns the player character (casted to a KidRedCharacter) object (NULL if the player pawn doesn't exist OR is not a character) at the specified player index
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static class AKidRedCharacter* GetKidRedPlayerCharacter(const UObject* WorldContextObject, int32 PlayerIndex);

	// Returns the player controller (casted to a KidRedController) at the specified player index
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static class AKidRedPlayerController* GetKidRedPlayerController(const UObject* WorldContextObject, int32 PlayerIndex);
};
