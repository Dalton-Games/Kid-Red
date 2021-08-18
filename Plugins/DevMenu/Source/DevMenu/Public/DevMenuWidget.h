// Copyright Void Interactive, 2021

#pragma once

#include "Blueprint/UserWidget.h"
#include "DevMenuWidget.generated.h"

USTRUCT(BlueprintType)
struct FDevMenu_ArrayCommand
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings", meta = (EditCondition = "bSupportsArrayGroups"))
	FText ArrayGroupName = FText::FromString("Array-0");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings")
    TArray<TSubclassOf<class UDevMenuScript>> DebugCommandClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings")
    TArray<class UDevMenuScript*> DebugCommands;
};

USTRUCT(BlueprintType)
struct FDevMenu_Command
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings")
	uint8 bIsGroup : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings", meta = (EditCondition = "bIsGroup"))
	FText GroupName = FText::FromString("Group-0");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings", meta = (EditCondition = "bIsGroup"))
	FMargin Padding = {0.0f, 5.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings")
    uint8 bSupportsArrayGroups : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings", meta = (EditCondition = "bSupportsArrayGroups"))
	TArray<FDevMenu_ArrayCommand> ArrayCommands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings")
	TArray<TSubclassOf<UDevMenuScript>> DebugCommandClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings")
    TArray<UDevMenuScript*> DebugCommands;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dev Menu Settings")
	TMap<FName, FString> CommandNameToConsoleCommand;

	friend bool operator ==(const FDevMenu_Command& Lhs, const FDevMenu_Command& Rhs)
	{
		return Lhs.GroupName.EqualTo(Rhs.GroupName);
	}
};

/**
 * A menu used to execute debug commands from a dev menu window
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class DEVMENU_API UDevMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeMenuName(const FName& NewMenuName);
	
	void InitializeMenuTitle();
	void InitializeMenuTitle(const FString& InMenuPrefixName, const FString& InMenuName, int32 InMenuIndex);
	void AssignMenuManager(class UDevMasterMenuWidgetBase* NewDevMenuManagerWidget);

	void SetWidgetSize(const FVector2D& NewWidgetSize);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dev Menu")
			void OnMenuOpened();
	virtual void OnMenuOpened_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dev Menu")
			void OnMenuClosed();
	virtual void OnMenuClosed_Implementation();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void AddDivider(const FText& InGroupName, FMargin InPadding, bool bIsEmpty = false);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void AddEmptyDivider();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void RemoveLastDivider();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
    class UDevCommandsDividerWidget* GetLastDivider();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void AddSubMenu(class UDevMenuWidget* NewSubMenu, bool bRefreshCommandList = true);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
    void AddDevCommand(const FDevMenu_Command& NewDevCommand, bool bRefreshCommandList = true);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void EmptySubMenusList();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void AddDefaultDebugCommands();

	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void AddExitButton();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
    void AddBackButton();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
    void AddFooterCommandButtons(bool bAddShortcutToMainMenu = false);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
    void AddMainMenuButton();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void EmptyCommandList();

	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void EmptyDevCommandsList();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void RefreshCommandList(bool bAddShortcutToMainMenu = false);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void ConfirmCommand();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void SelectCommand(int32 CommandIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
	void NextCommand();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
    void PreviousCommand();
	
	UFUNCTION(BlueprintCallable, Category = "Dev Menu")
    void DeselectAll();
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
    FORCEINLINE class UDevMasterMenuWidgetBase* GetDevMenuManager() const { return DevMenuManager; }
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	FORCEINLINE FText GetMenuName() const { return DevMenuName; }
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	TSubclassOf<class UDevCommandWidget> GetDevCommandWidgetClass() const;
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	bool ShouldForceRefreshOnMenuOpen() const;
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	bool ShouldCloseMenuOnCommandSelection() const;

	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	bool IsSubMenu(int32 InMenuIndex) const;
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	int32 GetNumCommands() const { return DevCommandWidgets.Num(); }
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	int32 GetNumDividers() const { return DividerWidgets.Num(); }
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu")
	bool IsUsingINI() const;

	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data")
	int32 MenuIndex = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data")
    int32 SelectionIndex = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data")
	uint8 bUsingINI : 1;
	
protected:
	void NativePreConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void PopulateCommandList(bool bAddShortcutToMainMenu);
	void PopulateCommandListFromINI(const FString& INIFilePath, bool bAddShortcutToMainMenu);

	class UDevCommandWidget* CreateDevCommandWidget(const FText& CommandName, UDevMenuScript* InCommandScript);
	class UDevCommandWidget_Array* CreateDevCommandWidget_Array(const FText& GroupName, TArray<UDevMenuScript*> InCommandScripts);

	class UDevMenuScript* CreateDebugMenuScript(TSubclassOf<UDevMenuScript> InDebugScriptClass);
	class UDevMenuScript_ConsoleCommand* CreateConsoleCommandScript(const FString& Command);
	class UDevMenuScript_OpenDebugMenu* CreateOpenDebugMenuScript();
	class UDevMenuScript_ExitMenu* CreateExitMenuScript();
	class UDevMenuScript_ExitToMainDevMenu* CreateExitToMainDevMenuScript();
	class UDevMenuScript_GoBack* CreateGoBackScript();
    
	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data", meta = (BindWidget))
	class UBorder* Border = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data", meta = (BindWidget))
	class UScrollBox* DebugCommands_ScrollBox = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data", meta = (BindWidget))
	class UTextBlock* DevMenuTitle_Text = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data")
	class UDevMasterMenuWidgetBase* DevMenuManager = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data")
	TArray<class UDevCommandWidget_Base*> DevCommandWidgets = {};
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu Data")
	TArray<class UDevCommandsDividerWidget*> DividerWidgets = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Menu Settings")
	FText DevMenuTitlePrefix = FText::FromString("Dev Menu");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Menu Settings")
	FText DevMenuName = FText::FromString("Menu");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Menu Settings")
    FVector2D WidgetSize = FVector2D(525.0f, 670.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Menu Settings")
	uint8 bForceRefreshOnOpen : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Menu Settings")
	uint8 bCloseMenuOnCommandSelection : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Menu Settings")
	TArray<FDevMenu_Command> DevMenuCommands = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dev Menu Settings")
	TArray<class UDevMenuWidget*> SubMenus = {};
	
	UPROPERTY(BlueprintReadOnly, Category = "Dev Menu")
	uint8 bMenuOpen : 1;
};
