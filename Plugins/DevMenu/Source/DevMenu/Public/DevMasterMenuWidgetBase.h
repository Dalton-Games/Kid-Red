// Copyright Void Interactive, 2021

#pragma once

#include "DevMenu.h"
#include "Blueprint/UserWidget.h"
#include "DevMasterMenuWidgetBase.generated.h"

/**
 * A base manager widget that contains every dev debug menu
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DEVMENU_API UDevMasterMenuWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	explicit UDevMasterMenuWidgetBase(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
	void ShowMenu();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
    void HideMenu();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
    void ResetStack();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
	bool ToggleMenuVisibility();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager", DisplayName = "Open Menu (Index)")
	void OpenMenu_Index(int32 MenuIndex);

	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager", DisplayName = "Open Menu (Name)")
	void OpenMenu_Name(FString InMenuName);

	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
	void OpenMenu(class UDevMenuWidget* InDevMenuWidget);

	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
	void PreviousMenu();

	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
	void SelectCommand();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
	void NextItem();

	UFUNCTION(BlueprintCallable, Category = "Dev Debug Menu Manager")
    void PreviousItem();
	
	UFUNCTION(BlueprintPure, Category = "Dev Debug Menu Manager")
	bool IsOpen();
	
	UFUNCTION(BlueprintPure, Category = "Dev Debug Menu Manager")
	bool IsClosed();
	
	UFUNCTION(BlueprintPure, Category = "Dev Debug Menu Manager")
	class UDevMenuWidget* GetMenu(int32 Index);
	
	bool ShouldAutoFillSubMenus() { return bAutoFillSubMenusOnConstruct; }

	UFUNCTION(BlueprintPure, Category = "Dev Debug Menu Manager")
	FORCEINLINE TArray<TSubclassOf<class UDevMenuScript>> GetDefaultDebugCommands() const { return DefaultDebugCommands; }
	
	UFUNCTION(BlueprintPure, Category = "Dev Debug Menu Manager")
	FORCEINLINE TMap<FName, FString> GetDefaultConsoleCommands() const { return DefaultConsoleCommands; }

	UFUNCTION(BlueprintPure, Category = "Dev Debug Menu Manager")
	FORCEINLINE int32 GetStackCount() const { return Stack.Num(); }

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Debug Menu Manager|Settings")
    uint8 bAutoFillSubMenusOnConstruct : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Debug Menu Manager|Settings")
	TArray<TSubclassOf<class UDevMenuScript>> DefaultDebugCommands;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Debug Menu Manager|Settings")
	TMap<FName, FString> DefaultConsoleCommands;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Debug Menu Manager|Required Widgets", meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Debug Menu Manager|Data")
	TArray<UWidget*> Stack;

private:
	void GetInputMode(UDevMenu::EInputMode& OutInputMode) const;
	UDevMenu::EInputMode OriginalInputMode = UDevMenu::EInputMode::GameOnly;
};

/**
* A manager widget that contains every dev debug menu
*/
UCLASS(Abstract, NotBlueprintable, Deprecated)
class DEVMENU_API UDEPRECATED_DevMasterMenuWidget : public UDevMasterMenuWidgetBase
{
	GENERATED_BODY()

public:
	explicit UDEPRECATED_DevMasterMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	void NativePreConstruct() override;
	void NativeConstruct() override;
};	