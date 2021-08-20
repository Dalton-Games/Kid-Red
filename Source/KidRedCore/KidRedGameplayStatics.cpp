// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedGameplayStatics.h"

AKidRedCharacter* UKidRedGameplayStatics::GetKidRedPlayerCharacter(const UObject* WorldContextObject, int32 PlayerIndex)
{
	AKidRedPlayerController* PC = GetKidRedPlayerController(WorldContextObject, PlayerIndex);
	return PC ? Cast<AKidRedCharacter>(PC->GetPawn()) : nullptr;
}

AKidRedPlayerController* UKidRedGameplayStatics::GetKidRedPlayerController(const UObject* WorldContextObject, const int32 PlayerIndex)
{
	return Cast<AKidRedPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex));
}
