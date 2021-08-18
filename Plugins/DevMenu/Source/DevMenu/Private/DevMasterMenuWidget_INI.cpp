// Copyright Void Interactive, 2021

#include "DevMasterMenuWidget_INI.h"

#include "DevMenuFunctionLibrary.h"
#include "DevMenuSettings.h"
#include "DevMenuWidget.h"

#include "Components/WidgetSwitcher.h"

#include "Misc/ConfigCacheIni.h"

void UDevMasterMenuWidget_INI::NativeConstruct()
{
	Super::NativeConstruct();

	GConfig->LoadFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir());

	TArray<FString> SectionNames = UDevMenuFunctionLibrary::GetAllSectionNamesFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir());
	int32 i = 0;
	for (const FString& SectionName : SectionNames)
	{
		FString CustomWidgetClassString = UDevMenuFunctionLibrary::GetStringFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), SectionName, "CustomWidgetClass");
		TSubclassOf<UDevMenuWidget> DevMenuClass = !CustomWidgetClassString.Contains("WidgetBlueprint") ? nullptr : LoadClass<UDevMenuWidget>(this, *CustomWidgetClassString);
		if (!DevMenuClass)
		{
			DevMenuClass = GetDefault<UDevMenuSettings>()->DevMenuWidgetClass;
		}

		if (DevMenuClass)
		{
			UDevMenuWidget* DevMenu = CreateWidget<UDevMenuWidget>(GetOwningPlayer(), DevMenuClass);

			const float WidgetWidth = UDevMenuFunctionLibrary::GetFloatFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), SectionName, "MenuWidth");
			const float WidgetHeight = UDevMenuFunctionLibrary::GetFloatFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), SectionName, "MenuHeight");
			
			DevMenu->SetWidgetSize({WidgetWidth, WidgetHeight});

			const FString TitlePrefix = UDevMenuFunctionLibrary::GetStringFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), SectionName, "TitlePrefix");
			DevMenu->InitializeMenuTitle(TitlePrefix, SectionName, i);
			DevMenu->InitializeMenuName(*SectionName);

			DevMenu->bUsingINI = true;
			DevMenu->MenuIndex = i;

			DevMenu->AssignMenuManager(this);

			MenuSwitcher->AddChild(DevMenu);

			i++;
		}
	}

	// Refresh each menu's command list
	TArray<UWidget*> ChildWidgets = MenuSwitcher->GetAllChildren();
	for (UWidget* ChildWidget : ChildWidgets)
	{
		UDevMenuWidget* DevMenuWidget = Cast<UDevMenuWidget>(ChildWidget);
		if (DevMenuWidget)
		{			
			DevMenuWidget->RefreshCommandList();
		}
	}

	Stack.Empty(1);

	Stack.Add(MenuSwitcher->GetWidgetAtIndex(0));
}
