// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectIterator.h"
#include "DevMenu.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDevMenu, All, All);

USTRUCT()
struct FDevMenuInputBinding
{
	GENERATED_BODY()

	TArray<FInputKeyBinding*> DevMenuNextKeyBindings;
	TArray<FInputKeyBinding*> DevMenuPreviousKeyBindings;
	TArray<FInputKeyBinding*> DevMenuSelectKeyBindings;
	TArray<FInputKeyBinding*> DevMenuBackKeyBindings;
	TArray<FInputKeyBinding*> DevMenuOpenKeyBindings;
};

UCLASS(NotBlueprintable, Transient)
class UDevMenu : public UObject
{
	GENERATED_BODY()

	UDevMenu();
	virtual ~UDevMenu() override;

	void BindKeys(APlayerController* InPlayerController);

public:
	static UDevMenu* Get();
	
	enum class EInputMode : uint8
	{
		GameAndUI,
		GameOnly,
		UIOnly
	};

	void Init(APlayerController* InPlayerController);
	
	bool IsOpen() const;
	
	void ToggleDevMenu();
	void NextDevCommand();
	void PreviousDevCommand();
	void SelectDevCommand();
	void PreviousMenu();

	UPROPERTY()
	class UDevMasterMenuWidgetBase* DevMasterMenu = nullptr;

	UPROPERTY()
	TMap<class APlayerController*, FDevMenuInputBinding> DevMenuInputBindingMap;
};

class FDevMenuModule : public IModuleInterface
{

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override;
	
	static UWorld* GetWorld();

protected:
	void RegisterSettings();
	void UnRegisterSettings();
	bool HandleSettingsSaved();
	
	void OnWorldInitialized(const UWorld::FActorsInitializedParams& Params);
	void OnPostWorldCleanUp(UWorld* World, bool bSessionEnded, bool bCleanupResources);
	
	void CreateDevMenu();
	void DestroyDevMenu();

	class UDevMenu* DevMenu = nullptr;
};
