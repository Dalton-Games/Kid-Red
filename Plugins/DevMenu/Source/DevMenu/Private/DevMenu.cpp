// Copyright Epic Games, Inc. All Rights Reserved.

#include "DevMenu.h"

#include "DevMenuFunctionLibrary.h"
#include "DevMenuSettings.h"
#include "ISettingsModule.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "HAL/FileManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#define LOCTEXT_NAMESPACE "FDevMenuModule"

void FDevMenuModule::StartupModule()
{
	RegisterSettings();

	#if !UE_BUILD_SHIPPING
	FWorldDelegates::OnWorldInitializedActors.AddRaw(this, &FDevMenuModule::OnWorldInitialized);
	FWorldDelegates::OnPostWorldCleanup.AddRaw(this, &FDevMenuModule::OnPostWorldCleanUp);

	// If there's no DevMenuCommands.ini in the project's config directory, make a copy from a template found in this module's directory
	if (!FPaths::FileExists(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir()))
	{
		IFileManager::Get().Copy(*UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), *UDevMenuFunctionLibrary::DevMenuCommandsPluginConfigDir(), false, true);
	}
	#endif
}

void FDevMenuModule::ShutdownModule()
{
	UnRegisterSettings();

	#if !UE_BUILD_SHIPPING
	FWorldDelegates::OnWorldInitializedActors.RemoveAll(this);
	FWorldDelegates::OnPostWorldCleanup.RemoveAll(this);
	
	DestroyDevMenu();
	#endif
}

bool FDevMenuModule::SupportsDynamicReloading()
{
	return true;
}

void FDevMenuModule::RegisterSettings()
{
	#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Dev Menu",
			LOCTEXT("RuntimeSettingsName", "Dev Menu"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the Dev Menu plugin"),
			GetMutableDefault<UDevMenuSettings>());
	}
	#endif
}

void FDevMenuModule::UnRegisterSettings()
{
	#if WITH_EDITOR
	if (UObjectInitialized())
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Plugins", "Dev Menu");
		}
	}
	#endif
}

bool FDevMenuModule::HandleSettingsSaved()
{
	#if WITH_EDITOR
	UDevMenuSettings* Settings = GetMutableDefault<UDevMenuSettings>();
	Settings->SaveConfig();
	#endif
	
	return true;
}

void FDevMenuModule::OnWorldInitialized(const UWorld::FActorsInitializedParams& Params)
{
	#if !UE_BUILD_SHIPPING
	if (Params.World)
	{
		Params.World->OnWorldBeginPlay.AddRaw(this, &FDevMenuModule::CreateDevMenu);
	}
	#endif
}

void FDevMenuModule::OnPostWorldCleanUp(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	#if !UE_BUILD_SHIPPING
	if (!World)
		return;

	DestroyDevMenu();
	#endif
}

void FDevMenuModule::CreateDevMenu()
{
	#if !UE_BUILD_SHIPPING
	if (!GetDefault<UDevMenuSettings>()->bEnabled)
		return;
	
	UWorld* World = GetWorld();
	if (!World)
		return;

	if (!World->GetGameState())
		return;

	if (DevMenu)
		return;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
	{
		DevMenu = UDevMenu::Get();
		DevMenu->Init(PC);
	}
	#endif
}

void FDevMenuModule::DestroyDevMenu()
{
	#if !UE_BUILD_SHIPPING
	if (DevMenu)
	{
		if (DevMenu->DevMasterMenu)
			DevMenu->DevMasterMenu->HideMenu();
		
		DevMenu->RemoveFromRoot();
		DevMenu->ConditionalBeginDestroy();
		DevMenu = nullptr;
	}
	#endif
}

UWorld* FDevMenuModule::GetWorld()
{
	if (!GEngine)
		return nullptr;

	const TIndirectArray<FWorldContext> WorldContexts = GEngine->GetWorldContexts();
	
	for (FWorldContext World : WorldContexts)
	{
		if (World.World() && World.World()->GetGameState())
		{
			return World.World();
		}
	}
	
	return nullptr;
}

UDevMenu::UDevMenu()
{
}

UDevMenu::~UDevMenu()
{
	DevMenuInputBindingMap.Empty();
}

UDevMenu* UDevMenu::Get()
{
	if (!GetDefault<UDevMenuSettings>()->bEnabled)
		return nullptr;
	
	for (const TObjectIterator<UDevMenu> It; It;)
		return *It;
	
	UDevMenu* DevMenu = NewObject<UDevMenu>();
	DevMenu->AddToRoot();
	return DevMenu;
}

void UDevMenu::Init(APlayerController* InPlayerController)
{
	if (!GetDefault<UDevMenuSettings>()->bEnabled)
		return;
	
	if (!InPlayerController)
		return;

	if (DevMasterMenu)
		return;

	if (InPlayerController->IsLocalPlayerController())
	{
		BindKeys(InPlayerController);
		
		DevMasterMenu = CreateWidget<UDevMasterMenuWidgetBase>(InPlayerController, GetDefault<UDevMenuSettings>()->DevMasterMenuWidgetClass);
		if (DevMasterMenu)
		{
			DevMasterMenu->AddToViewport(99999999);
		}
	}
}

void UDevMenu::BindKeys(APlayerController* InPlayerController)
{
	if (!InPlayerController)
		return;

	DevMenuInputBindingMap.Add(InPlayerController);
	
	if (InPlayerController->InputComponent)
	{
		if (DevMenuInputBindingMap[InPlayerController].DevMenuOpenKeyBindings.Num() == 0)
		{
			for (const FInputChord& InputChord : GetDefault<UDevMenuSettings>()->DevMenuKeys)
			{
				FInputKeyBinding* PressedBinding = &InPlayerController->InputComponent->BindKey(InputChord, IE_Pressed, this, &UDevMenu::ToggleDevMenu);
				PressedBinding->bConsumeInput = true;
				PressedBinding->bExecuteWhenPaused = true;
				
				DevMenuInputBindingMap[InPlayerController].DevMenuOpenKeyBindings.AddUnique(PressedBinding);
			}
		}
			
		if (DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.Num() == 0)
		{
			for (const FKey& InputKey : GetDefault<UDevMenuSettings>()->NextCommandKeys)
			{
				FInputKeyBinding* PressedBinding = &InPlayerController->InputComponent->BindKey(InputKey, IE_Pressed, this, &UDevMenu::NextDevCommand);
				PressedBinding->bConsumeInput = true;
				PressedBinding->bExecuteWhenPaused = true;

				FInputKeyBinding* RepeatBinding = &InPlayerController->InputComponent->BindKey(InputKey, IE_Repeat, this, &UDevMenu::NextDevCommand);
				RepeatBinding->bConsumeInput = true;
				RepeatBinding->bExecuteWhenPaused = true;
				
				DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.AddUnique(PressedBinding);
				DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.AddUnique(RepeatBinding);
			}
		}

		if (DevMenuInputBindingMap[InPlayerController].DevMenuPreviousKeyBindings.Num() == 0)
		{
			for (const FKey& InputKey : GetDefault<UDevMenuSettings>()->PreviousCommandKeys)
			{
				FInputKeyBinding* PressedBinding = &InPlayerController->InputComponent->BindKey(InputKey, IE_Pressed, this, &UDevMenu::PreviousDevCommand);
				PressedBinding->bConsumeInput = true;
				PressedBinding->bExecuteWhenPaused = true;

				FInputKeyBinding* RepeatBinding = &InPlayerController->InputComponent->BindKey(InputKey, IE_Repeat, this, &UDevMenu::PreviousDevCommand);
				RepeatBinding->bConsumeInput = true;
				RepeatBinding->bExecuteWhenPaused = true;
				
				DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.AddUnique(PressedBinding);
				DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.AddUnique(RepeatBinding);
			}
		}

		if (DevMenuInputBindingMap[InPlayerController].DevMenuSelectKeyBindings.Num() == 0)
		{
			for (const FKey& InputKey : GetDefault<UDevMenuSettings>()->SelectCommandKeys)
			{
				FInputKeyBinding* PressedBinding = &InPlayerController->InputComponent->BindKey(InputKey, IE_Pressed, this, &UDevMenu::SelectDevCommand);
				PressedBinding->bConsumeInput = true;
				PressedBinding->bExecuteWhenPaused = true;

				DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.AddUnique(PressedBinding);
			}
		}

		if (DevMenuInputBindingMap[InPlayerController].DevMenuBackKeyBindings.Num() == 0)
		{
			for (const FKey& InputKey : GetDefault<UDevMenuSettings>()->PreviousMenuKeys)
			{
				FInputKeyBinding* PressedBinding = &InPlayerController->InputComponent->BindKey(InputKey, IE_Pressed, this, &UDevMenu::PreviousMenu);
				PressedBinding->bConsumeInput = true;
				PressedBinding->bExecuteWhenPaused = true;

				FInputKeyBinding* RepeatBinding = &InPlayerController->InputComponent->BindKey(InputKey, IE_Repeat, this, &UDevMenu::PreviousMenu);
				RepeatBinding->bConsumeInput = true;
				RepeatBinding->bExecuteWhenPaused = true;
				
				DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.AddUnique(PressedBinding);
				DevMenuInputBindingMap[InPlayerController].DevMenuNextKeyBindings.AddUnique(RepeatBinding);
			}
		}

		//UE_LOG(LogDevMenu, Warning, TEXT("%i"), InPlayerController->InputComponent->KeyBindings.Num());
	}
}

bool UDevMenu::IsOpen() const
{
	return DevMasterMenu && DevMasterMenu->IsOpen();
}

void UDevMenu::ToggleDevMenu()
{
	if (!DevMasterMenu)
	{
		Init(UGameplayStatics::GetPlayerController(FDevMenuModule::GetWorld(), 0));
	}

	if (DevMasterMenu)
	{
		DevMasterMenu->ToggleMenuVisibility();
	}
}

void UDevMenu::NextDevCommand()
{
	if (DevMasterMenu)
	{
		DevMasterMenu->NextItem();
	}
}

void UDevMenu::PreviousDevCommand()
{
	if (DevMasterMenu)
	{
		DevMasterMenu->PreviousItem();
	}
}

void UDevMenu::SelectDevCommand()
{
	if (DevMasterMenu)
	{
		DevMasterMenu->SelectCommand();
	}
}

void UDevMenu::PreviousMenu()
{
	if (DevMasterMenu)
	{
		if (DevMasterMenu->GetStackCount() > 1)
			DevMasterMenu->PreviousMenu();
		else
			DevMasterMenu->HideMenu();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDevMenuModule, DevMenu)

DEFINE_LOG_CATEGORY(LogDevMenu)
