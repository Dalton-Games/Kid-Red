// Copyright Void Interactive, 2021

#pragma once

#include "DevMenuScript.generated.h"

UENUM(BlueprintType)
enum class EDevCommandBehaviour : uint8
{
	DCB_Normal		UMETA(DisplayName="Normal"),
	DCB_MonoState	UMETA(DisplayName="Mono State"),
    DCB_DualState	UMETA(DisplayName="Dual State"),
    DCB_MultiState	UMETA(DisplayName="Multi State"),
    DCB_Editable	UMETA(DisplayName="Editable"),
};

/**
 * A script that executes debug commands initiated by a dev menu
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class DEVMENU_API UDevMenuScript : public UObject/*, public FTickableGameObject*/
{
	GENERATED_BODY()

public:
	explicit UDevMenuScript(const FObjectInitializer& ObjectInitializer);

    void NativeInitialize(class UDevMenuWidget* InDevMenuOwner, class UDevCommandWidget* InDevCommandWidgetOwner);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug Menu Script")
    void Initialize(class UDevMenuWidget* InDevMenuOwner, class UDevCommandWidget* InDevCommandWidgetOwner);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug Menu Script")
	bool IsAllowedOnDevMenu();

	void NativeExecuteScript();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug Menu Script")
    void ExecuteScript();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug Menu Script")
	FText GetStateValue();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug Menu Script")
	FLinearColor GetStateValueTextColor();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Debug Menu Script")
	bool UpdateValueTextEveryFrame() const;
	
	UFUNCTION(BlueprintCallable, Category = "Debug Menu Script")
	FORCEINLINE FText GetCommandName() const { return DebugCommandName; }
	
	UFUNCTION(BlueprintPure, Category = "Debug Menu Script")
	FORCEINLINE EDevCommandBehaviour GetCommandBehaviour() const { return CommandBehaviour; }

	UFUNCTION(BlueprintPure, Category = "Debug Menu Script")
    FORCEINLINE TArray<FText> GetStateValues() const { return StateValues; }
	
	UFUNCTION(BlueprintPure, Category = "Debug Menu Script")
	FORCEINLINE bool ShouldCloseMenuOnExecute() const { return bCloseMenuOnExecute; }
	
protected:
	/*virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;*/

	virtual UWorld* GetWorld() const override;

	virtual void Initialize_Implementation(class UDevMenuWidget* InDevMenuOwner, class UDevCommandWidget* InDevCommandWidgetOwner);
	virtual	bool IsAllowedOnDevMenu_Implementation();
	virtual	bool UpdateValueTextEveryFrame_Implementation();
    virtual void ExecuteScript_Implementation();
	virtual FText GetStateValue_Implementation();
	virtual FLinearColor GetStateValueTextColor_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Menu Script", meta = (ExposeOnSpawn=true))
	FText DebugCommandName = FText::FromString("None");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug Menu Script")
	EDevCommandBehaviour CommandBehaviour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Menu Script", meta = (EditCondition = "CommandBehaviour == EDevCommandBehaviour::DCB_MultiState"))
	TArray<FText> StateValues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug Menu Script")
	uint8 bCloseMenuOnExecute : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Debug Menu Script", meta = (ExposeOnSpawn=true))
	TArray<UDevMenuScript*> DebugMenuScriptReferences;

	UPROPERTY(BlueprintReadOnly, Category = "Debug Menu Script")
	class UDevCommandWidget* DevCommandWidgetOwner = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Debug Menu Script")
	class UDevMenuWidget* DevMenuOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Debug Menu Script")
    class UWorld* World = nullptr;
};

/**
* A script that tells a dev menu to go "up"/"back" a menu
*/
UCLASS(NotBlueprintable, NotPlaceable)
class DEVMENU_API UDevMenuScript_GoBack : public UDevMenuScript
{
	GENERATED_BODY()

public:
	explicit UDevMenuScript_GoBack(const FObjectInitializer& ObjectInitializer);
	
protected:
	void ExecuteScript_Implementation() override;
};

/**
* 
*/
UCLASS(NotBlueprintable, NotPlaceable)
class DEVMENU_API UDevMenuScript_OpenDebugMenu : public UDevMenuScript
{
	GENERATED_BODY()

public:
	explicit UDevMenuScript_OpenDebugMenu(const FObjectInitializer& ObjectInitializer);

	void SetMenuToOpen(class UDevMenuWidget* InDevMenuToOpen);
	
protected:
    void ExecuteScript_Implementation() override;

	UPROPERTY()
	class UDevMenuWidget* DevMenuToOpen = nullptr;
};

/**
* 
*/
UCLASS(NotBlueprintable, NotPlaceable)
class DEVMENU_API UDevMenuScript_ExitMenu : public UDevMenuScript
{
	GENERATED_BODY()

public:
    explicit UDevMenuScript_ExitMenu(const FObjectInitializer& ObjectInitializer);
	
protected:
    void ExecuteScript_Implementation() override;
};

/**
* 
*/
UCLASS(NotBlueprintable, NotPlaceable)
class DEVMENU_API UDevMenuScript_ExitToMainDevMenu : public UDevMenuScript
{
	GENERATED_BODY()

public:
    explicit UDevMenuScript_ExitToMainDevMenu(const FObjectInitializer& ObjectInitializer);
	
protected:
    void ExecuteScript_Implementation() override;
};

/**
* 
*/
UCLASS(NotBlueprintable, NotPlaceable)
class DEVMENU_API UDevMenuScript_ConsoleCommand : public UDevMenuScript
{
	GENERATED_BODY()

public:
    explicit UDevMenuScript_ConsoleCommand(const FObjectInitializer& ObjectInitializer);

	void SetDebugCommandName(const FString& NewCommandName) { DebugCommandName = FText::FromString(NewCommandName); }
	
	UPROPERTY(BlueprintReadOnly, Category = "Debug Menu Script|Console Command")
	FString ConsoleCommand = "None";
	
protected:
    void ExecuteScript_Implementation() override;
};
