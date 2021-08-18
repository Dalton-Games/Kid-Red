// Copyright Void Interactive, 2021

#pragma once

#include "Blueprint/UserWidget.h"
#include "DevCommandWidget.generated.h"

UCLASS(Abstract, BlueprintType, NotBlueprintable)
class DEVMENU_API UDevCommandWidget_Base : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Dev Command Widget", DisplayName = "Initialize Widget")
	void InitializeWidget(class UDevMenuWidget* InDevMenuOwner, class UDevCommandsDividerWidget* InDividerWidgetOwner, class UDevMenuScript* InCommandScript, const TArray<class UDevMenuScript*>& InCommandScripts);

	UFUNCTION(BlueprintPure, Category = "Dev Command Widget")
	virtual bool IsAllowedOnDevMenu() const { return true; }
	
	UFUNCTION(BlueprintCallable, Category = "Dev Command Widget")
	virtual bool ExecuteCommand() { return false; }
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget")
	int32 MenuIndex = 0;

	FORCEINLINE class UDevCommandsDividerWidget* GetDividerWidget() { return DividerWidgetOwner; }
	
	void OnCommandButtonPressed();
	void OnCommandButtonSelected();
	void OnCommandButtonUnselected();
	
	virtual void ShowSelectionArrow() {}
	virtual void HideSelectionArrow() {}
	
protected:
	void NativeConstruct() override;

	virtual void InitializeWidget_Implementation(class UDevMenuWidget* InDevMenuOwner, class UDevCommandsDividerWidget* InDividerWidgetOwner, class UDevMenuScript* InCommandScript, const TArray<class UDevMenuScript*>& InCommandScripts);


	UFUNCTION()
	void OnCommandButtonClicked();

	UFUNCTION()
	void OnCommandButtonHovered();

	UFUNCTION()
	void OnCommandButtonUnhovered();

	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget")
	class UDevMenuWidget* DevMenuOwner;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget")
    class UDevCommandsDividerWidget* DividerWidgetOwner;
};

/**
 * A widget that displays a command and executes it when clicked
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DEVMENU_API UDevCommandWidget : public UDevCommandWidget_Base
{
	GENERATED_BODY()

public:
	void InitializeWidget_Implementation(class UDevMenuWidget* InDevMenuOwner, class UDevCommandsDividerWidget* InDividerWidgetOwner, class UDevMenuScript* InCommandScript, const TArray<class UDevMenuScript*>& InCommandScripts) override;

	bool IsAllowedOnDevMenu() const override;

	bool ExecuteCommand() override;

	UFUNCTION(BlueprintCallable, Category = "Dev Command Widget")
	void SetCommandName(const FText& NewCommandName);
	
	UFUNCTION(BlueprintPure, Category = "Dev Command Widget")
    FText GetCommandName() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Command Widget", meta = (ExposeOnSpawn = true))
    FText DebugCommandName = FText::FromString("None");

	FORCEINLINE class UDevMenuScript* GetDebugMenuScript() { return CommandScript; }
	FORCEINLINE class UButton* GetButton() { return Command_Button; }

	void ShowSelectionArrow() override;
	void HideSelectionArrow() override;
	
protected:
	void NativeConstruct() override;
	void NativeOnInitialized() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget", meta = (BindWidget))
    class UTextBlock* CommandName_Text = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget", meta = (BindWidget))
    class UButton* Command_Button = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget", meta = (BindWidget))
    class UWidgetSwitcher* ValueWidgetSwitcher = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget", meta = (BindWidget))
    class UTextBlock* Value_Text = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget", meta = (BindWidget))
    class UEditableTextBox* EditableValue_Text = nullptr;
    
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget", meta = (BindWidget))
    class UImage* SelectionArrow = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Command Widget")
    FLinearColor ArrowSelectedColor = FColor::Yellow;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Command Widget")
    FLinearColor ArrowDeselectedColor = FColor::Silver;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget")
    class UDevMenuScript* CommandScript = nullptr;
};

/**
* A widget that displays a list of commands and executes one when clicked
*/
UCLASS(Abstract, BlueprintType, Blueprintable)
class DEVMENU_API UDevCommandWidget_Array : public UDevCommandWidget_Base
{
	GENERATED_BODY()

public:
	void InitializeWidget_Implementation(class UDevMenuWidget* InDevMenuOwner, class UDevCommandsDividerWidget* InDividerWidgetOwner, class UDevMenuScript* InCommandScript, const TArray<class UDevMenuScript*>& InCommandScripts) override;

	bool IsAllowedOnDevMenu() const override;

	void SetGroupName(const FText& InGroupName);

protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget (Array)", meta = (BindWidget))
	class UVerticalBox* Command_Container = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget (Array)", meta = (BindWidget))
	class UDevCommandsDividerWidget* DevCommand_Divider = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Command Widget (Array)")
	TArray<class UDevCommandWidget*> CommandWidgets;
};
