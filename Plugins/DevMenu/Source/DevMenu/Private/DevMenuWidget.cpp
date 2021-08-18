// Copyright Void Interactive, 2021

#include "DevMenuWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

#include "DevMasterMenuWidgetBase.h"
#include "DevCommandsDividerWidget.h"
#include "DevCommandWidget.h"
#include "DevMenuScript.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "DevMenuFunctionLibrary.h"
#include "DevMenuSettings.h"

void UDevMenuWidget::InitializeMenuName(const FName& NewMenuName)
{
	DevMenuName = FText::FromName(NewMenuName);
}

void UDevMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsDesignTime())
		SetWidgetSize(WidgetSize);
}

void UDevMenuWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (int32 i = 0; i < DevCommandWidgets.Num(); i++)
	{
		UDevCommandWidget_Base* Widget = DevCommandWidgets[i];
		if (Widget)
		{
			if (UDevCommandsDividerWidget* DividerWidget = Widget->GetDividerWidget())
			{
				if (!DividerWidget->IsCollapsed())
					Widget->SetVisibility(Widget->IsAllowedOnDevMenu() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			}
			else
			{
				Widget->SetVisibility(Widget->IsAllowedOnDevMenu() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			}
		}
	}

	for (int32 i = 0; i < DevCommandWidgets.Num(); i++)
	{
		UDevCommandWidget_Base* Widget = DevCommandWidgets[i];
		if (Widget)
		{
			if (UDevCommandsDividerWidget* DividerWidget = Widget->GetDividerWidget())
			{
				DividerWidget->SetVisibility(DividerWidget->AreAllDevCommandsCollapsed() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
			}
		}
	}
}

void UDevMenuWidget::SetWidgetSize(const FVector2D& NewWidgetSize)
{
	UCanvasPanelSlot* BorderCanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Border);
	if (BorderCanvasPanelSlot)
		BorderCanvasPanelSlot->SetSize(NewWidgetSize);
}

UDevCommandWidget* UDevMenuWidget::CreateDevCommandWidget(const FText& CommandName, UDevMenuScript* InCommandScript)
{
	if (GetDefault<UDevMenuSettings>()->DevCommandWidgetClass)
	{
		UDevCommandWidget* DevCommand = CreateWidget<UDevCommandWidget>(GetOwningPlayer(), GetDefault<UDevMenuSettings>()->DevCommandWidgetClass);
		if (DevCommand)
		{
			DebugCommands_ScrollBox->AddChild(DevCommand);
			
			DevCommand->SetPadding(FMargin(10.0f, 0.0f, 10.0f, 0.0f));
			DevCommand->SetCommandName(CommandName);
			DevCommand->MenuIndex = DevCommandWidgets.Num();
			DevCommand->SetVisibility(ESlateVisibility::Collapsed);

			DevCommand->InitializeWidget(this, GetLastDivider(), InCommandScript, {});
						
			DevCommandWidgets.Add(DevCommand);
		}

		return DevCommand;
	}

	return nullptr;
}

UDevCommandWidget_Array* UDevMenuWidget::CreateDevCommandWidget_Array(const FText& GroupName, const TArray<UDevMenuScript*> InCommandScripts)
{
	if (GetDefault<UDevMenuSettings>()->DevCommandArrayWidgetClass)
	{
		UDevCommandWidget_Array* DevCommand = CreateWidget<UDevCommandWidget_Array>(GetOwningPlayer(), GetDefault<UDevMenuSettings>()->DevCommandArrayWidgetClass);
		if (DevCommand)
		{
			DebugCommands_ScrollBox->AddChild(DevCommand);
			
			DevCommand->SetPadding(FMargin(10.0f, 0.0f, 10.0f, 0.0f));
			DevCommand->SetGroupName(GroupName);
			DevCommand->MenuIndex = DevCommandWidgets.Num();

			DevCommand->InitializeWidget(this, GetLastDivider(), nullptr, InCommandScripts);
						
			DevCommandWidgets.Add(DevCommand);
		}

		return DevCommand;
	}

	return nullptr;
}

void UDevMenuWidget::InitializeMenuTitle()
{
	const FString TitleSuffix = DevMenuTitlePrefix.ToString() + " (" + DevMenuName.ToString() + ":" + FString::FromInt(MenuIndex) + ")";
	DevMenuTitle_Text->SetText(FText::FromString(TitleSuffix));
}

void UDevMenuWidget::InitializeMenuTitle(const FString& InMenuPrefixName, const FString& InMenuName, int32 InMenuIndex)
{
	const FString TitleSuffix = InMenuPrefixName + " (" + InMenuName + ":" + FString::FromInt(InMenuIndex) + ")";
	DevMenuTitle_Text->SetText(FText::FromString(TitleSuffix));
}

void UDevMenuWidget::AssignMenuManager(UDevMasterMenuWidgetBase* NewDevMenuManagerWidget)
{
	DevMenuManager = NewDevMenuManagerWidget;
}

void UDevMenuWidget::OnMenuOpened_Implementation()
{
	bMenuOpen = true;
}

void UDevMenuWidget::OnMenuClosed_Implementation()
{
	bMenuOpen = false;
}

void UDevMenuWidget::AddDivider(const FText& InGroupName, const FMargin InPadding, const bool bIsEmpty)
{
	if (GetDefault<UDevMenuSettings>()->DevCommandDividerWidgetClass)
	{
		UDevCommandsDividerWidget* DividerWidget = CreateWidget<UDevCommandsDividerWidget>(GetOwningPlayer(), GetDefault<UDevMenuSettings>()->DevCommandDividerWidgetClass);
		DebugCommands_ScrollBox->AddChild(DividerWidget);
		DividerWidget->SetGroupName(InGroupName);
		DividerWidget->SetIsEmpty(bIsEmpty);
		DividerWidget->SetPadding(InPadding);

		DividerWidgets.Add(DividerWidget);
	}
}

void UDevMenuWidget::AddEmptyDivider()
{
	AddDivider(FText::FromString(""), FMargin(0.0f), true);
}

void UDevMenuWidget::RemoveLastDivider()
{
	UDevCommandsDividerWidget* LastDividerWidget = GetLastDivider();
	if (LastDividerWidget)
	{
		DividerWidgets.Remove(LastDividerWidget);
		LastDividerWidget->RemoveFromParent();
	}
}

UDevCommandsDividerWidget* UDevMenuWidget::GetLastDivider()
{
	if (!DebugCommands_ScrollBox)
		return nullptr;
	
	UDevCommandsDividerWidget* LastDividerWidget = nullptr;
	for (UWidget* ChildWidget : DebugCommands_ScrollBox->GetAllChildren())
	{
		UDevCommandsDividerWidget* DividerWidget = Cast<UDevCommandsDividerWidget>(ChildWidget);
		if (DividerWidget)
		{
			LastDividerWidget = DividerWidget;
		}
	}

	return LastDividerWidget;
}

void UDevMenuWidget::AddSubMenu(UDevMenuWidget* NewSubMenu, const bool bRefreshCommandList)
{
	SubMenus.Add(NewSubMenu);

	if (bRefreshCommandList)
		RefreshCommandList();
}

void UDevMenuWidget::AddDevCommand(const FDevMenu_Command& NewDevCommand, const bool bRefreshCommandList)
{
	DevMenuCommands.AddUnique(NewDevCommand);

	if (bRefreshCommandList)
		RefreshCommandList(DevMenuManager ? DevMenuManager->GetStackCount() >= 3 : false);
}

void UDevMenuWidget::AddExitButton()
{
	if (DevMenuCommands.Num() > 0)
	{
		AddEmptyDivider();
	}

	UDevMenuScript_ExitMenu* ExitMenu_Script = CreateExitMenuScript();
	CreateDevCommandWidget(ExitMenu_Script->GetCommandName(), ExitMenu_Script);
}

void UDevMenuWidget::AddBackButton()
{
	UDevMenuScript_GoBack* GoBack_Script = CreateGoBackScript();
	CreateDevCommandWidget(GoBack_Script->GetCommandName(), GoBack_Script);
}

void UDevMenuWidget::AddFooterCommandButtons(const bool bAddShortcutToMainMenu)
{
	if (DebugCommands_ScrollBox->GetChildAt(DebugCommands_ScrollBox->GetChildrenCount() - 1) != GetLastDivider())
	{
		AddEmptyDivider();
	}

	if (bAddShortcutToMainMenu)
		AddMainMenuButton();
	
	AddBackButton();
}

void UDevMenuWidget::AddMainMenuButton()
{
	UDevMenuScript_ExitToMainDevMenu* ExitToMainDevMenu_Script = CreateExitToMainDevMenuScript();
	CreateDevCommandWidget(ExitToMainDevMenu_Script->GetCommandName(), ExitToMainDevMenu_Script);
}

void UDevMenuWidget::EmptyCommandList()
{
	UDevMenuFunctionLibrary::RemoveFromParentAndClear(DevCommandWidgets);
}

void UDevMenuWidget::EmptyDevCommandsList()
{
	DevMenuCommands.Empty();
}

void UDevMenuWidget::EmptySubMenusList()
{
	SubMenus.Empty();
}

void UDevMenuWidget::AddDefaultDebugCommands()
{
	if (DevMenuManager)
	{
		for (auto ConsoleCommand : DevMenuManager->GetDefaultConsoleCommands())
		{
			CreateDevCommandWidget(FText::FromName(ConsoleCommand.Key), CreateConsoleCommandScript(ConsoleCommand.Value));
		}
		
		for (TSubclassOf<UDevMenuScript> DebugScriptClass : DevMenuManager->GetDefaultDebugCommands())
		{
			CreateDevCommandWidget(DebugScriptClass.GetDefaultObject()->GetCommandName(), CreateDebugMenuScript(DebugScriptClass));
		}
	}
}

void UDevMenuWidget::RefreshCommandList(const bool bAddShortcutToMainMenu)
{
	#if UE_BUILD_SHIPPING
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
	return;
	#endif
	
	UDevMenuFunctionLibrary::RemoveFromParentAndClear(DevCommandWidgets);
	UDevMenuFunctionLibrary::RemoveFromParentAndClear(DividerWidgets);

	if (DebugCommands_ScrollBox)
		DebugCommands_ScrollBox->ClearChildren();

	PopulateCommandList(bAddShortcutToMainMenu);
	PopulateCommandListFromINI(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), bAddShortcutToMainMenu);

	if (MenuIndex > 0)
	{
		AddFooterCommandButtons(bAddShortcutToMainMenu);
	}

	SelectCommand(0);
}

void UDevMenuWidget::PopulateCommandList(const bool bAddShortcutToMainMenu)
{
	if (DevMenuCommands.Num() > 0)
	{
		DevMenuCommands.RemoveAll([](const FDevMenu_Command& Element)
		{
			return Element.DebugCommandClasses.Num() == 0 && Element.DebugCommands.Num() == 0 && Element.CommandNameToConsoleCommand.Num() == 0 && Element.ArrayCommands.Num() == 0;
		});
	}

	// Add all submenus
	{
		int32 i = 0;
		for (UDevMenuWidget* SubMenu : SubMenus)
		{
			if (SubMenu)
			{
				FString CommandName = SubMenu->GetMenuName().ToString() + "...";
				UDevMenuScript_OpenDebugMenu* OpenDebugMenu_Script = CreateOpenDebugMenuScript();
				OpenDebugMenu_Script->SetMenuToOpen(SubMenu);

				CreateDevCommandWidget(FText::FromString(CommandName), OpenDebugMenu_Script);
			}
			else
			{
				#if WITH_EDITOR
				UE_LOG(LogDevMenu, Error, TEXT("Failed to create sub-menu: %s's sub-menu at index %i is null."), *GetName(), i);
				#endif
			}
			
			i++;
		}
	}

	// Add all the debug commands
	for (const FDevMenu_Command& DevMenuCommand : DevMenuCommands)
	{
		if (DevMenuCommand.bIsGroup)
		{
			RemoveLastDivider();

			AddDivider(DevMenuCommand.GroupName, DevMenuCommand.Padding);
		}

		// Blueprint commands (class ref)
		{
			TArray<UDevCommandWidget_Base*> DevCommandWidgetsInThisScope;
			
			for (TSubclassOf<UDevMenuScript> DebugScriptClass : DevMenuCommand.DebugCommandClasses)
			{
				if (DebugScriptClass)
				{
					UDevCommandWidget* DevCommand = CreateDevCommandWidget(DebugScriptClass.GetDefaultObject()->GetCommandName(), CreateDebugMenuScript(DebugScriptClass));
					if (DevCommand)
					{
						DevCommandWidgetsInThisScope.Add(DevCommand);
					}
				}
			}

			if (DevMenuCommand.bSupportsArrayGroups)
			{
				TArray<UDevMenuScript*> CommandScripts;
				for (const FDevMenu_ArrayCommand& ArrayCommand : DevMenuCommand.ArrayCommands)
				{
					for (TSubclassOf<UDevMenuScript> DebugScriptClass : ArrayCommand.DebugCommandClasses)
					{
						if (DebugScriptClass)
						{
							UDevMenuScript* Script = CreateDebugMenuScript(DebugScriptClass);
							if (Script)
							{
								CommandScripts.Add(Script);
							}
						}
					}
					
					if (CommandScripts.Num() > 0)
					{
						UDevCommandWidget_Array* DevCommand = CreateDevCommandWidget_Array(ArrayCommand.ArrayGroupName, CommandScripts);
						if (DevCommand)
						{
							DevCommandWidgetsInThisScope.Add(DevCommand);
						}
					}

					CommandScripts.Empty();
				}
			}

			if (DevMenuCommand.bIsGroup)
			{
				UDevCommandsDividerWidget* LastDividerWidget = GetLastDivider();
				if (LastDividerWidget)
				{
					LastDividerWidget->AddDevCommands(DevCommandWidgetsInThisScope);
				}
			}
		}

		// Blueprint commands (object ref)
		{
			TArray<UDevCommandWidget_Base*> DevCommandWidgetsInThisScope;
			
			for (UDevMenuScript* DebugScript : DevMenuCommand.DebugCommands)
			{
				if (DebugScript)
				{
					UDevCommandWidget* DevCommand = CreateDevCommandWidget(DebugScript->GetCommandName(), DebugScript);
					if (DevCommand)
					{
						DevCommandWidgetsInThisScope.Add(DevCommand);
					}
				}
			}

			if (DevMenuCommand.bSupportsArrayGroups)
			{
				for (const FDevMenu_ArrayCommand& ArrayCommand : DevMenuCommand.ArrayCommands)
				{
					if (ArrayCommand.DebugCommands.Num() > 0)
					{
						UDevCommandWidget_Array* DevCommand = CreateDevCommandWidget_Array(ArrayCommand.ArrayGroupName, ArrayCommand.DebugCommands);
						if (DevCommand)
						{
							DevCommandWidgetsInThisScope.Add(DevCommand);
						}
					}
				}
			}

			if (DevMenuCommand.bIsGroup)
			{
				UDevCommandsDividerWidget* LastDividerWidget = GetLastDivider();
				if (LastDividerWidget)
				{
					LastDividerWidget->AddDevCommands(DevCommandWidgetsInThisScope);
				}
			}
		}

		// Console commands
		{
			TArray<UDevCommandWidget_Base*> DevCommandWidgetsInThisScope;
			for (auto CommandMap : DevMenuCommand.CommandNameToConsoleCommand)
			{
				UDevCommandWidget* DevCommand = CreateDevCommandWidget(FText::FromName(CommandMap.Key), CreateConsoleCommandScript(CommandMap.Value));
				if (DevCommand)
				{
					DevCommandWidgetsInThisScope.Add(DevCommand);
				}
			}

			if (DevMenuCommand.bIsGroup)
			{
				UDevCommandsDividerWidget* LastDividerWidget = GetLastDivider();
				if (LastDividerWidget)
				{
					LastDividerWidget->AddDevCommands(DevCommandWidgetsInThisScope);
				}
			}
		}

		AddDivider(FText::FromString(""), DevMenuCommand.Padding, true);
	}
}

void UDevMenuWidget::PopulateCommandListFromINI(const FString& INIFilePath, bool bAddShortcutToMainMenu)
{
	const TArray<FString> SectionNames = UDevMenuFunctionLibrary::GetAllSectionNamesFromINIFile(INIFilePath);
	
	// Add all submenus
	{
		const TArray<FString> SubMenusFromINI = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "SubMenu");
		for (const FString& SubMenu : SubMenusFromINI)
		{
			if (!SubMenu.IsEmpty())
			{
				FString CommandName = SubMenu + "...";
				UDevMenuScript_OpenDebugMenu* OpenDebugMenu_Script = CreateOpenDebugMenuScript();
				OpenDebugMenu_Script->SetMenuToOpen(DevMenuManager->GetMenu(SectionNames.Find(SubMenu)));

				CreateDevCommandWidget(FText::FromString(CommandName), OpenDebugMenu_Script);
			}
		}

		if (SubMenusFromINI.Num() > 0)
			AddEmptyDivider();
	}

	const TArray<FString> GroupNames = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "Group");
	
	// Add all debug commands
	{
		int32 i = 0;
		for (const FString& GroupName : GroupNames)
		{
			if (!GroupName.IsEmpty())
			{
				RemoveLastDivider();
				
				if (GroupName.Contains("NoNameGroup"))
				{
					if (GetNumDividers() > 0 || i > 0)
						AddDivider(FText::FromString(""), Padding, true); //todo padding from ini file
				}
				else
				{
					AddDivider(FText::FromString(GroupName), Padding); //todo padding from ini file
				}

				TArray<UDevCommandWidget_Base*> DevCommandWidgetsInThisScope;

				// Blueprint commands
				const TArray<FString> DebugCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "DebugCommand");
				for (const FString& DebugCommandEntry : DebugCommands)
				{
					if (!DebugCommandEntry.IsEmpty())
					{
						TArray<FString> DebugCommandInfo;
						DebugCommandEntry.ParseIntoArray(DebugCommandInfo, TEXT(","));

						TSubclassOf<UDevMenuScript> DebugScriptClass = nullptr;

						if (DebugCommandInfo.Num() > 1)
						{
							const bool bGroupNamesMatch = DebugCommandInfo[0] == GroupName;
							if (bGroupNamesMatch)
							{
								DebugScriptClass = StaticLoadClass(UDevMenuScript::StaticClass(), nullptr, *DebugCommandInfo[1]);
								if (DebugScriptClass)
								{
									UDevCommandWidget* DevCommand = CreateDevCommandWidget(DebugScriptClass.GetDefaultObject()->GetCommandName(), CreateDebugMenuScript(DebugScriptClass));
									if (DevCommand)
									{
										DevCommandWidgetsInThisScope.Add(DevCommand);
									}
								}
							}
						}
					}
				}

				// Console commands
				const TArray<FString> ConsoleCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "ConsoleCommand");
				for (const FString& ConsoleCommandEntry : ConsoleCommands)
				{
					if (!ConsoleCommandEntry.IsEmpty())
					{
						TArray<FString> ConsoleCommandInfo;
						ConsoleCommandEntry.ParseIntoArray(ConsoleCommandInfo, TEXT(","));
						
						if (ConsoleCommandInfo.Num() > 2)
						{
							const bool bGroupNamesMatch = ConsoleCommandInfo[0] == GroupName;
							if (bGroupNamesMatch)
							{
								UDevCommandWidget* DevCommand = CreateDevCommandWidget(FText::FromString(ConsoleCommandInfo[1]), CreateConsoleCommandScript(ConsoleCommandInfo[2]));
								if (DevCommand)
								{
									DevCommandWidgetsInThisScope.Add(DevCommand);
								}
							}
						}
					}
				}

				// Array commands
				const TArray<FString> ArrayCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "Array");
				for (const FString& ArrayCommandEntry : ArrayCommands)
				{
					if (!ArrayCommandEntry.IsEmpty())
					{
						TArray<FString> DebugCommandInfo;
						ArrayCommandEntry.ParseIntoArray(DebugCommandInfo, TEXT(","));

						TSubclassOf<UDevMenuScript> DebugScriptClass = nullptr;

						if (DebugCommandInfo.Num() > 2)
						{
							const bool bGroupNamesMatch = DebugCommandInfo[0] == GroupName;
							if (bGroupNamesMatch)
							{
								TArray<UDevMenuScript*> CommandScripts;
								for (int32 j = 2; j < DebugCommandInfo.Num(); j++)
								{
									if (DebugCommandInfo[j].Contains("Blueprint'"))
									{
										DebugScriptClass = StaticLoadClass(UDevMenuScript::StaticClass(), nullptr, *DebugCommandInfo[j]);
										if (DebugScriptClass)
										{
											CommandScripts.Add(CreateDebugMenuScript(DebugScriptClass));
										}
									}
									else
									{
										UDevMenuScript_ConsoleCommand* ConsoleCommandScript = CreateConsoleCommandScript(DebugCommandInfo[i]);
										if (ConsoleCommandScript)
										{
											ConsoleCommandScript->SetDebugCommandName(DebugCommandInfo[i]);
								
											CommandScripts.Add(ConsoleCommandScript);
										}
									}
								}

								if (CommandScripts.Num() > 0)
								{
									UDevCommandWidget_Array* DevCommandArray = CreateDevCommandWidget_Array(FText::FromString(DebugCommandInfo[1]), CommandScripts);
									if (DevCommandArray)
									{
										DevCommandWidgetsInThisScope.Add(DevCommandArray);
									}
								}
							}
						}
					}
				}

				UDevCommandsDividerWidget* LastDividerWidget = GetLastDivider();
				if (LastDividerWidget)
				{
					LastDividerWidget->AddDevCommands(DevCommandWidgetsInThisScope);
				}

				if (DevCommandWidgetsInThisScope.Num() == 0)
					RemoveLastDivider();
			}

			AddDivider(FText::FromString(""), Padding, true); //todo padding from ini file

			i++;
		}

		// If we had at least 1 command in a group, add an empty divider
		if (i > 0)
		{
			RemoveLastDivider();
			AddEmptyDivider();
		}

		i = 0;
			
		// No group debug commands
		const TArray<FString> DebugCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "DebugCommand");
		for (const FString& DebugCommandEntry : DebugCommands)
		{
			if (!DebugCommandEntry.IsEmpty())
			{
				TArray<FString> DebugCommandInfo;
				DebugCommandEntry.ParseIntoArray(DebugCommandInfo, TEXT(","));

				TSubclassOf<UDevMenuScript> DebugScriptClass = nullptr;

				if (DebugCommandInfo.Num() == 1 || (DebugCommandInfo[0].IsEmpty() && DebugCommandInfo.Num() == 2))
				{
					DebugScriptClass = StaticLoadClass(UDevMenuScript::StaticClass(), nullptr, *DebugCommandInfo[0]);
					if (DebugScriptClass)
					{
						CreateDevCommandWidget(DebugScriptClass.GetDefaultObject()->GetCommandName(), CreateDebugMenuScript(DebugScriptClass));
						i++;
					}
				}
			}
		}
		
		// No group console commands
		const TArray<FString> ConsoleCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "ConsoleCommand");
		for (const FString& ConsoleCommandEntry : ConsoleCommands)
		{
			if (!ConsoleCommandEntry.IsEmpty())
			{
				TArray<FString> ConsoleCommandInfo;
				ConsoleCommandEntry.ParseIntoArray(ConsoleCommandInfo, TEXT(","));

				if (ConsoleCommandInfo.Num() == 2)
				{
					CreateDevCommandWidget(FText::FromString(ConsoleCommandInfo[0]), CreateConsoleCommandScript(ConsoleCommandInfo[1]));
					i++;
				}
			}
		}

		// No group array commands
		//const TArray<FString> ArrayCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "Array");
		//for (const FString& ArrayCommandEntry : ArrayCommands)
		//{
		//	if (!ArrayCommandEntry.IsEmpty())
		//	{
		//		TArray<FString> DebugCommandInfo;
		//		ArrayCommandEntry.ParseIntoArray(DebugCommandInfo, TEXT(","));
		//
		//		TSubclassOf<UDebugMenuScript> DebugScriptClass = nullptr;
		//
		//		if (DebugCommandInfo.Num() > 1)
		//		{
		//			TArray<UDebugMenuScript*> CommandScripts;
		//			for (int32 j = 1; j < DebugCommandInfo.Num(); j++)
		//			{
		//				if (DebugCommandInfo[j].Contains("Blueprint'"))
		//				{
		//					DebugScriptClass = StaticLoadClass(UDebugMenuScript::StaticClass(), nullptr, *DebugCommandInfo[j]);
		//					if (DebugScriptClass)
		//					{
		//						CommandScripts.Add(CreateDebugMenuScript(DebugScriptClass));
		//					}
		//				}
		//				else
		//				{
		//					UDevMenuScript_ConsoleCommand* ConsoleCommandScript = CreateConsoleCommandScript(DebugCommandInfo[j]);
		//					if (ConsoleCommandScript)
		//					{
		//						ConsoleCommandScript->SetDebugCommandName(DebugCommandInfo[j]);
		//						
		//						CommandScripts.Add(ConsoleCommandScript);
		//					}
		//				}
		//			}
		//
		//			if (CommandScripts.Num() > 0)
		//			{
		//				CreateDevCommandWidget_Array(FText::FromString(DebugCommandInfo[0]), CommandScripts);
		//			}
		//		}
		//	}
		//}

		//if (MenuIndex > 0)
		//{
			//AddFooterCommandButtons(bAddShortcutToMainMenu);
		//}
		//else
		//{
			if (i == 0)
			{
				RemoveLastDivider();
				AddEmptyDivider();
			}

			// Add default console commands
			const TArray<FString> DefaultConsoleCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "DefaultConsoleCommand");
			for (const FString& ConsoleCommandEntry : DefaultConsoleCommands)
			{
				if (!ConsoleCommandEntry.IsEmpty())
				{
					TArray<FString> ConsoleCommandInfo;
					ConsoleCommandEntry.ParseIntoArray(ConsoleCommandInfo, TEXT(","));

					if (ConsoleCommandInfo.Num() == 2)
					{
						CreateDevCommandWidget(FText::FromString(ConsoleCommandInfo[0]), CreateConsoleCommandScript(ConsoleCommandInfo[1]));
					}
				}
			}
			
			// Add default debug commands
			const TArray<FString> DefaultDebugCommands = UDevMenuFunctionLibrary::GetStringArrayFromINIFile(INIFilePath, DevMenuName.ToString(), "DefaultDebugCommand");
			for (const FString& DebugCommandEntry : DefaultDebugCommands)
			{
				if (!DebugCommandEntry.IsEmpty())
				{
					TSubclassOf<UDevMenuScript> DebugScriptClass = StaticLoadClass(UDevMenuScript::StaticClass(), nullptr, *DebugCommandEntry);
					if (DebugScriptClass)
						CreateDevCommandWidget(DebugScriptClass.GetDefaultObject()->GetCommandName(), CreateDebugMenuScript(DebugScriptClass));
				}
			}
		//}
	}
}

UDevMenuScript* UDevMenuWidget::CreateDebugMenuScript(const TSubclassOf<UDevMenuScript> InDebugScriptClass)
{
	if (InDebugScriptClass)
		return NewObject<UDevMenuScript>(this, InDebugScriptClass.Get(), InDebugScriptClass->GetFName(), RF_NoFlags, InDebugScriptClass.GetDefaultObject(), true);

	return nullptr;
}

UDevMenuScript_ConsoleCommand* UDevMenuWidget::CreateConsoleCommandScript(const FString& Command)
{
	UDevMenuScript_ConsoleCommand* ConsoleCommandScript = NewObject<UDevMenuScript_ConsoleCommand>(this);
	if (ConsoleCommandScript)
		ConsoleCommandScript->ConsoleCommand = Command;

	return ConsoleCommandScript;
}

UDevMenuScript_OpenDebugMenu* UDevMenuWidget::CreateOpenDebugMenuScript()
{
	return NewObject<UDevMenuScript_OpenDebugMenu>(this);
}

UDevMenuScript_ExitMenu* UDevMenuWidget::CreateExitMenuScript()
{
	return NewObject<UDevMenuScript_ExitMenu>(this);
}

UDevMenuScript_ExitToMainDevMenu* UDevMenuWidget::CreateExitToMainDevMenuScript()
{
	return NewObject<UDevMenuScript_ExitToMainDevMenu>(this);
}

UDevMenuScript_GoBack* UDevMenuWidget::CreateGoBackScript()
{
	return NewObject<UDevMenuScript_GoBack>(this);
}

void UDevMenuWidget::ConfirmCommand()
{
	if (DevCommandWidgets.Num() == 0)
		return;

	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(TakeWidget()));

	DevCommandWidgets[SelectionIndex]->OnCommandButtonPressed();
}

void UDevMenuWidget::SelectCommand(const int32 CommandIndex)
{
	if (DevCommandWidgets.Num() == 0)
		return;

	DeselectAll();
	
	SelectionIndex = FMath::Clamp(CommandIndex, 0, DevCommandWidgets.Num()-1);
	
	DevCommandWidgets[SelectionIndex]->ShowSelectionArrow();
}

void UDevMenuWidget::NextCommand()
{
	if (DevCommandWidgets.Num() == 0)
		return;

	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(TakeWidget()));

	// Recursion fail-safe
	{
		bool bAllInvisible = true;
		for (UDevCommandWidget_Base* DevCommandWidget : DevCommandWidgets)
		{
			if (DevCommandWidget && DevCommandWidget->IsVisible())
			{
				bAllInvisible = false;
				break;
			}
		}

		if (bAllInvisible)
			return;
	}

	DevCommandWidgets[SelectionIndex]->HideSelectionArrow();

	SelectionIndex++;

	if (SelectionIndex > DevCommandWidgets.Num()-1)
	{
		SelectionIndex = 0;
	}
	
	if (DevCommandWidgets[SelectionIndex]->IsVisible())
	{
		DevCommandWidgets[SelectionIndex]->ShowSelectionArrow();

		DebugCommands_ScrollBox->ScrollWidgetIntoView(DevCommandWidgets[SelectionIndex]);
	}
	else
	{
		NextCommand();
	}
}

void UDevMenuWidget::PreviousCommand()
{
	if (DevCommandWidgets.Num() == 0)
		return;

	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(TakeWidget()));

	// Recursion fail-safe
	{
		bool bAllInvisible = true;
		for (UDevCommandWidget_Base* DevCommandWidget : DevCommandWidgets)
		{
			if (DevCommandWidget && DevCommandWidget->IsVisible())
			{
				bAllInvisible = false;
				break;
			}
		}

		if (bAllInvisible)
			return;
	}

	DevCommandWidgets[SelectionIndex]->HideSelectionArrow();

	SelectionIndex--;
	
	if (SelectionIndex < 0)
	{
		SelectionIndex = DevCommandWidgets.Num()-1;
	}

	if (DevCommandWidgets[SelectionIndex]->IsVisible())
	{
		DevCommandWidgets[SelectionIndex]->ShowSelectionArrow();

		DebugCommands_ScrollBox->ScrollWidgetIntoView(DevCommandWidgets[SelectionIndex]);
	}
	else
	{
		PreviousCommand();
	}
}

void UDevMenuWidget::DeselectAll()
{
	for (UDevCommandWidget_Base* DevCommandWidget : DevCommandWidgets)
	{
		DevCommandWidget->HideSelectionArrow();
	}
}

TSubclassOf<UDevCommandWidget> UDevMenuWidget::GetDevCommandWidgetClass() const
{
	return GetDefault<UDevMenuSettings>()->DevCommandWidgetClass;
}

bool UDevMenuWidget::ShouldForceRefreshOnMenuOpen() const
{
	return bForceRefreshOnOpen || UDevMenuFunctionLibrary::GetBoolFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), DevMenuName.ToString(), "bForceRefreshOnOpen");
}

bool UDevMenuWidget::ShouldCloseMenuOnCommandSelection() const
{
	return bCloseMenuOnCommandSelection || UDevMenuFunctionLibrary::GetBoolFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), DevMenuName.ToString(), "bCloseMenuOnCommandSelection");
}

bool UDevMenuWidget::IsSubMenu(const int32 InMenuIndex) const
{
	return InMenuIndex < SubMenus.Num();
}

bool UDevMenuWidget::IsUsingINI() const
{
	return bUsingINI && !UDevMenuFunctionLibrary::GetBoolFromINIFile(UDevMenuFunctionLibrary::DevMenuCommandsConfigDir(), DevMenuName.ToString(), "bPopulateCommandsFromWidget");
}
