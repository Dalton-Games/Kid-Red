// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedSkeletalMeshComponent.h"

UKidRedSkeletalMeshComponent::UKidRedSkeletalMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickInterval = TICK_RATE_60;

	bEnableUpdateRateOptimizations = false;
}
