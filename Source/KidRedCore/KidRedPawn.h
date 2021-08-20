// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "KidRedPawn.generated.h"

UCLASS()
class KIDREDCORE_API AKidRedPawn : public APawn
{
	GENERATED_BODY()

public:
	AKidRedPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
