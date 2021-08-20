// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "Item.h"
#include "Grenade.generated.h"

USTRUCT(BlueprintType)
struct FGrenadeItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetonationDamage = 25.0f;
};

USTRUCT(BlueprintType)
struct FGrenadeItemDataTable : public FItemDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade Item Table", meta = (DisplayAfter = "ItemData"))
	FGrenadeItemData GrenadeItemData;
};

/**
 * Base class for all grenade items
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class KIDREDITEMS_API AGrenade : public AItem
{
	GENERATED_BODY()

public:
	AGrenade();
	
	UFUNCTION(BlueprintPure, DisplayName = "Get Grenade Data", Category = "Item")
	FORCEINLINE FGrenadeItemData GetGrenadeData_Copy() const { return GrenadeItemData != nullptr ? *GrenadeItemData : FGrenadeItemData(); }
	
protected:
	virtual void OnItemRegistryFound(const FDataRegistryAcquireResult& RegistryAcquireResult) override;

	const FGrenadeItemData* GrenadeItemData = nullptr;
};
