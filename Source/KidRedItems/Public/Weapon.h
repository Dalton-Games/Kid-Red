// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "Item.h"
#include "Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 15.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FireRate = 0.1f;
};

USTRUCT(BlueprintType)
struct FWeaponItemDataTable : public FItemDataTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Item Table", meta = (DisplayAfter = "ItemData"))
	FWeaponItemData WeaponItemData;
};

/**
 * Base class for all weapon items
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class KIDREDITEMS_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();
	
	UFUNCTION(BlueprintPure, DisplayName = "Get Weapon Data", Category = "Item")
	FORCEINLINE FWeaponItemData GetWeaponData_Copy() const { return WeaponItemData; }
	FORCEINLINE FWeaponItemData& GetWeaponData_Copy() { return WeaponItemData; }

protected:
	virtual void OnItemRegistryFound(const FDataRegistryAcquireResult& RegistryAcquireResult) override;

	FWeaponItemData WeaponItemData;
};
