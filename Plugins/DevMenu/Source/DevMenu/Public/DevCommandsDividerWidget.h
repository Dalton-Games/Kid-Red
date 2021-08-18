// Copyright Void Interactive, 2021

#pragma once

#include "Blueprint/UserWidget.h"
#include "DevCommandsDividerWidget.generated.h"

/**
 * A widget that acts as a divider between groups of commands on a dev menu
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DEVMENU_API UDevCommandsDividerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	explicit UDevCommandsDividerWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Dev Commands Divider Widget")
	bool ToggleCommandVisibility();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Commands Divider Widget")
    void ExpandCommands();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Commands Divider Widget")
	void CollapseCommands();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Commands Divider Widget")
	void AssignDevCommands(const TArray<class UDevCommandWidget_Base*>& InDevCommandWidgets);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Commands Divider Widget")
	void AddDevCommands(const TArray<class UDevCommandWidget_Base*>& InDevCommandWidgets);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Commands Divider Widget")
	void SetGroupName(const FText& NewGroupName);

	UFUNCTION(BlueprintCallable, Category = "Dev Commands Divider Widget")
	void SetIsEmpty(bool bIsEmpty);
	
	UFUNCTION(BlueprintPure, Category = "Dev Commands Divider Widget")
	FORCEINLINE bool IsSimpleDivider() const { return bIsSimpleDivider; }
	
	UFUNCTION(BlueprintPure, Category = "Dev Commands Divider Widget")
	FORCEINLINE bool IsCollapsed() const { return bIsCollapsed; }
	
	UFUNCTION(BlueprintPure, Category = "Dev Commands Divider Widget")
	bool AreAllDevCommandsCollapsed() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Dev Commands Divider Widget | Data", meta = (BindWidget))
	class UTextBlock* GroupName_Text = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Commands Divider Widget | Data")
	TArray<class UDevCommandWidget_Base*> DevCommandWidgets;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Commands Divider Widget | Data")
	uint8 bIsCollapsed : 1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Commands Divider Widget | Data")
	uint8 bIsSimpleDivider : 1;
};
