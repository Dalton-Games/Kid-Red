// Copyright 2021 Kid Red. All Rights Reserved.

#include "Grenade.h"

AGrenade::AGrenade()
{
	ItemRegistryType = "GrenadesRegistry";
}

void AGrenade::OnItemRegistryFound(const FDataRegistryAcquireResult& RegistryAcquireResult)
{
	Super::OnItemRegistryFound(RegistryAcquireResult);
	
	const FGrenadeItemData& GrenadeItemDataFromTable = RegistryAcquireResult.GetItem<FGrenadeItemDataTable>()->GrenadeItemData;
	
	GrenadeItemData = GrenadeItemDataFromTable;
}
