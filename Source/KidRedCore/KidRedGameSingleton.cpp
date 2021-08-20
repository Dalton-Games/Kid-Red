// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedGameSingleton.h"

UKidRedGameSingleton& UKidRedGameSingleton::Get()
{
	return *GetKidRedGameSingleton();
}

UKidRedGameSingleton* UKidRedGameSingleton::GetKidRedGameSingleton()
{
	return Cast<UKidRedGameSingleton>(GEngine->GameSingleton);
}
