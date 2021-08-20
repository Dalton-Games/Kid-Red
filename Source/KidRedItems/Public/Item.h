// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayPriority = 1), Category = "Item Data")
	FName ItemName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayPriority = 2), Category = "Item Data")
	FText ItemNameUI = FText::FromString("None");
};

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Table")
	FItemData ItemData;
};

/**
 * Base class for all items in the game
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class KIDREDITEMS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	UFUNCTION(BlueprintPure, DisplayName = "Get Item Data", Category = "Item")
	FORCEINLINE FItemData GetItemData_Copy() const { return ItemData; }
	FORCEINLINE FItemData& GetItemData() { return ItemData; }

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnItemRegistryFound(const FDataRegistryAcquireResult& RegistryAcquireResult);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Registry")
	FName ItemRegistryType = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Registry")
	FName TableRowName = NAME_None;
	
	FItemData ItemData;
	
private:
	void ApplyItemDataFromRegistry(const FName& InRegistryType);
};
