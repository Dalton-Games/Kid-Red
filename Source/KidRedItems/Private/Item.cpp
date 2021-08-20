// Copyright 2021 Kid Red. All Rights Reserved.

#include "Item.h"

#include "Components/SkeletalMeshComponent.h"

#include "DataRegistrySubsystem.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = TICK_RATE_60;

	bFindCameraComponentWhenViewTarget = false;
	SetCanBeDamaged(false);

	SetReplicates(true);
	bAlwaysRelevant = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(Mesh);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	ApplyItemDataFromRegistry(ItemRegistryType);
}

void AItem::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::ApplyItemDataFromRegistry(const FName& InRegistryType)
{
	FDataRegistryItemAcquiredCallback Delegate;
	Delegate.BindUObject(this, &AItem::OnItemRegistryFound);

	FDataRegistryId RegistryId;
	RegistryId.RegistryType = InRegistryType;
	RegistryId.ItemName = TableRowName;
	
	if (const UDataRegistrySubsystem* DataRegistrySubsystem = UDataRegistrySubsystem::Get())
	{
		if (!DataRegistrySubsystem->AcquireItem(RegistryId, Delegate))
		{
			#if !UE_BUILD_SHIPPING
			ULog::Error(GetName() + ": Failed to apply item data. Registry item (" + TableRowName.ToString() + ") " + "was not found in registry (" + InRegistryType.ToString() + ")");
			#endif
		}
	}
}

void AItem::OnItemRegistryFound(const FDataRegistryAcquireResult& RegistryAcquireResult)
{
	const FItemData& ItemDataFromTable = RegistryAcquireResult.GetItem<FItemDataTable>()->ItemData;
	
	ItemData = &ItemDataFromTable;
}
