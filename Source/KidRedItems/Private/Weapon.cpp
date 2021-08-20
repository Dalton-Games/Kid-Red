// Copyright 2021 Kid Red. All Rights Reserved.

#include "Weapon.h"

AWeapon::AWeapon()
{
	ItemRegistryType = "WeaponsRegistry";
}

void AWeapon::OnItemRegistryFound(const FDataRegistryAcquireResult& RegistryAcquireResult)
{
	Super::OnItemRegistryFound(RegistryAcquireResult);

	const FWeaponItemData& WeaponItemDataFromTable = RegistryAcquireResult.GetItem<FWeaponItemDataTable>()->WeaponItemData;
	
	WeaponItemData = WeaponItemDataFromTable;
}
