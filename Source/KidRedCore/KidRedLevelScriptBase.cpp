// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedLevelScriptBase.h"

AKidRedLevelScriptBase::AKidRedLevelScriptBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = TICK_RATE_30;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	bFindCameraComponentWhenViewTarget = false;
	bEnableAutoLODGeneration = false;

	SetCanBeDamaged(false);
}
