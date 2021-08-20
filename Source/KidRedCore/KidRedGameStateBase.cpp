// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedGameStateBase.h"

AKidRedGameStateBase::AKidRedGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.0f;
}
