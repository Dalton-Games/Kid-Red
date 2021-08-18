// Copyright Ali El Saleh 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DevMenu/Public/DevMasterMenuWidgetBase.h"
#include "DevMenu/Public/DevMenuWidget.h"
#include "DevMenu/Public/DevCommandsDividerWidget.h"
#include "DevMenu/Public/DevCommandWidget.h"
#include "DevMenuSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class DEVMENU_API UDevMenuSettings final : public UObject
{
	GENERATED_BODY()

public:
	UDevMenuSettings();
	
	UPROPERTY(EditAnywhere, config, Category = "General")
	uint8 bEnabled : 1;
	
	UPROPERTY(EditAnywhere, config, Category = "General", meta = (EditCondition = "bEnabled"))
	TArray<FInputChord> DevMenuKeys;
	
	UPROPERTY(EditAnywhere, config, Category = "General", meta = (EditCondition = "bEnabled"))
	TArray<FKey> NextCommandKeys;
	
	UPROPERTY(EditAnywhere, config, Category = "General", meta = (EditCondition = "bEnabled"))
	TArray<FKey> PreviousCommandKeys;
	
	UPROPERTY(EditAnywhere, config, Category = "General", meta = (EditCondition = "bEnabled"))
	TArray<FKey> SelectCommandKeys;
	
	UPROPERTY(EditAnywhere, config, Category = "General", meta = (EditCondition = "bEnabled"))
	TArray<FKey> PreviousMenuKeys;

	UPROPERTY(EditAnywhere, config, Category = "Required", meta = (EditCondition = "bEnabled"))
    TSubclassOf<class UDevMasterMenuWidgetBase> DevMasterMenuWidgetClass = nullptr;
	
	UPROPERTY(EditAnywhere, config, Category = "Required", meta = (EditCondition = "bEnabled"))
    TSubclassOf<class UDevMenuWidget> DevMenuWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, config, Category = "Required", meta = (EditCondition = "bEnabled"))
	TSubclassOf<class UDevCommandWidget> DevCommandWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, config, Category = "Required", meta = (EditCondition = "bEnabled"))
	TSubclassOf<class UDevCommandWidget_Array> DevCommandArrayWidgetClass = nullptr;
	
	UPROPERTY(EditAnywhere, config, Category = "Required", meta = (EditCondition = "bEnabled"))
	TSubclassOf<class UDevCommandsDividerWidget> DevCommandDividerWidgetClass = nullptr;
};
