// Copyright Void Interactive, 2021

#include "DevMasterMenuWidgetBase.h"

#include "Components/WidgetSwitcher.h"

#include "DevMenuWidget.h"
#include "DevMenuScript.h"

UDevMasterMenuWidgetBase::UDevMasterMenuWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultDebugCommands.Empty(1);
	DefaultDebugCommands.Add(UDevMenuScript_ExitMenu::StaticClass());
}

void UDevMasterMenuWidgetBase::GetInputMode(UDevMenu::EInputMode& OutInputMode) const
{
	if (UGameViewportClient* GameViewportClient = GetWorld()->GetGameViewport())
	{
		if (GameViewportClient->IgnoreInput())
		{
			OutInputMode = UDevMenu::EInputMode::UIOnly;
		}
		else
		{
			const bool bIsGameOnlyInput = GameViewportClient->GetMouseCaptureMode() == EMouseCaptureMode::CapturePermanently || GameViewportClient->GetMouseCaptureMode() == EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown; 
			if (bIsGameOnlyInput)
			{
				OutInputMode = UDevMenu::EInputMode::GameOnly;
			}
			else
			{
				OutInputMode = UDevMenu::EInputMode::GameAndUI;
			}
		}
	}
}

void UDevMasterMenuWidgetBase::ShowMenu()
{
	if (IsVisible())
		return;
	
	#if !UE_BUILD_SHIPPING
	// Remember what input mode we were on before overriding
	GetInputMode(OriginalInputMode);
	
	SetVisibility(ESlateVisibility::Visible);
	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(TakeWidget()));
	GetOwningPlayer()->bShowMouseCursor = true;
	#else
	HideMenu();
	#endif
}

void UDevMasterMenuWidgetBase::HideMenu()
{
	if (!IsVisible())
		return;
	
	SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->SetInputMode(FInputModeGameOnly().SetConsumeCaptureMouseDown(true));

	// todo: make setting, disabled by default
	//switch (OriginalInputMode)
	//{
	//	case UDevMenu::EInputMode::GameAndUI:
	//	GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
	//	break;
	//
	//	case UDevMenu::EInputMode::GameOnly:
	//	GetOwningPlayer()->SetInputMode(FInputModeGameOnly().SetConsumeCaptureMouseDown(true));
	//	break;
	//
	//	case UDevMenu::EInputMode::UIOnly:
	//	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
	//	break;
	//
	//	default:
	//	GetOwningPlayer()->SetInputMode(FInputModeGameOnly().SetConsumeCaptureMouseDown(true));
	//	break;
	//}

	GetOwningPlayer()->bShowMouseCursor = false;
}

void UDevMasterMenuWidgetBase::ResetStack()
{
	for (int32 i = 0; i < Stack.Num(); i++)
	{
		PreviousMenu();
	}
}

bool UDevMasterMenuWidgetBase::ToggleMenuVisibility()
{
	if (IsVisible())
	{
		HideMenu();
		return false;
	}

	ShowMenu();
	return true;
}

void UDevMasterMenuWidgetBase::OpenMenu_Index(const int32 MenuIndex)
{
	if (MenuIndex > -1)
	{
		MenuSwitcher->SetActiveWidgetIndex(MenuIndex);

		UDevMenuWidget* CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);
		if (CurrentDevMenuWidget->ShouldForceRefreshOnMenuOpen())
		{
			// Empty current list of commands
			if (CurrentDevMenuWidget)
				CurrentDevMenuWidget->EmptyCommandList();
		}

		Stack.Insert(MenuSwitcher->GetWidgetAtIndex(MenuIndex), 0);

		CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);
		if (CurrentDevMenuWidget->ShouldForceRefreshOnMenuOpen())
		{
			// Add new list of commands
			CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);
			if (CurrentDevMenuWidget)
				CurrentDevMenuWidget->RefreshCommandList();
		}
	}

	if (Stack.Num() >= 3)
	{
		Cast<UDevMenuWidget>(Stack[0])->RefreshCommandList(true);
	}
}

void UDevMasterMenuWidgetBase::OpenMenu_Name(const FString InMenuName)
{
	UDevMenuWidget* DevMenuWidgetToOpen = nullptr;
	
	for (auto Widget : MenuSwitcher->GetAllChildren())
	{
		UDevMenuWidget* CurrentDevMenuWidget = Cast<UDevMenuWidget>(Widget);
		if (CurrentDevMenuWidget)
		{
			if (CurrentDevMenuWidget->GetMenuName().ToString() == InMenuName)
			{
				DevMenuWidgetToOpen = CurrentDevMenuWidget;
				break;
			}
		}
	}

	OpenMenu(DevMenuWidgetToOpen);
}

void UDevMasterMenuWidgetBase::OpenMenu(UDevMenuWidget* InDevMenuWidget)
{
	if (InDevMenuWidget)
	{
		MenuSwitcher->SetActiveWidget(InDevMenuWidget);

		// Empty current list of commands
		UDevMenuWidget* CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);
		if (CurrentDevMenuWidget->ShouldForceRefreshOnMenuOpen())
		{
			if (CurrentDevMenuWidget)
				CurrentDevMenuWidget->EmptyCommandList();
		}

		Stack.Insert(InDevMenuWidget, 0);

		InDevMenuWidget->OnMenuOpened();

		if (InDevMenuWidget->ShouldForceRefreshOnMenuOpen())
		{
			// Add new list of commands
			if (InDevMenuWidget)
				InDevMenuWidget->RefreshCommandList();
		}
	}

	if (Stack.Num() >= 3)
	{
		Cast<UDevMenuWidget>(Stack[0])->RefreshCommandList(true);
	}
}

void UDevMasterMenuWidgetBase::PreviousMenu()
{
	if (Stack.Num() > 1)
	{
		UDevMenuWidget* CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);

		if (CurrentDevMenuWidget->ShouldForceRefreshOnMenuOpen())
		{
			// Empty current list of commands
			if (CurrentDevMenuWidget)
				CurrentDevMenuWidget->EmptyCommandList();
		}

		Stack.RemoveAt(0);
		MenuSwitcher->SetActiveWidget(Stack[0]);

		CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);
		CurrentDevMenuWidget->OnMenuClosed();

		if (CurrentDevMenuWidget->ShouldForceRefreshOnMenuOpen())
		{
			// Add new list of commands
			CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);
			if (CurrentDevMenuWidget)
				CurrentDevMenuWidget->RefreshCommandList();
		}

		GetOwningPlayer()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(CurrentDevMenuWidget->TakeWidget()));
	}
}

void UDevMasterMenuWidgetBase::SelectCommand()
{
	if (!IsVisible())
		return;
	
	if (Stack.Num() > 0)
	{
		UDevMenuWidget* CurrentDevMenuWidget = Cast<UDevMenuWidget>(Stack[0]);
		if (CurrentDevMenuWidget)
		{
			CurrentDevMenuWidget->ConfirmCommand();
		}
	}
}

void UDevMasterMenuWidgetBase::NextItem()
{
	if (!IsVisible())
		return;
	
	if (Stack.Num() > 0)
	{
		Cast<UDevMenuWidget>(Stack[0])->NextCommand();
	}
}

void UDevMasterMenuWidgetBase::PreviousItem()
{
	if (!IsVisible())
		return;
	
	if (Stack.Num() > 0)
	{
		Cast<UDevMenuWidget>(Stack[0])->PreviousCommand();
	}
}

bool UDevMasterMenuWidgetBase::IsOpen()
{
	return IsVisible();
}

bool UDevMasterMenuWidgetBase::IsClosed()
{
	return !IsVisible();
}

UDevMenuWidget* UDevMasterMenuWidgetBase::GetMenu(const int32 Index)
{
	return Cast<UDevMenuWidget>(MenuSwitcher->GetWidgetAtIndex(Index));
}

void UDevMasterMenuWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);

	#if UE_BUILD_SHIPPING
	RemoveFromParent();
	return;
	#endif
}

UDEPRECATED_DevMasterMenuWidget::UDEPRECATED_DevMasterMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDEPRECATED_DevMasterMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	TArray<UWidget*> ChildWidgets_SubMenus = MenuSwitcher->GetAllChildren();
	ChildWidgets_SubMenus.RemoveAt(0);
	
	int32 i = 0;
	for (UWidget* ChildWidget : MenuSwitcher->GetAllChildren())
	{
		UDevMenuWidget* DevMenuWidget = Cast<UDevMenuWidget>(ChildWidget);
		if (DevMenuWidget)
		{
			// Assign all sub-menus to the main dev menu
			// 0 should be Main dev menu
			if (i == 0 && ShouldAutoFillSubMenus())
			{
				DevMenuWidget->EmptySubMenusList();
				
				for (UWidget* ChildWidget_SubMenu : ChildWidgets_SubMenus)
				{
					UDevMenuWidget* SubDevMenuWidget = Cast<UDevMenuWidget>(ChildWidget_SubMenu);
					if (SubDevMenuWidget)
					{
						DevMenuWidget->AddSubMenu(SubDevMenuWidget, false);
					}
				}
			}
			
			DevMenuWidget->MenuIndex = i;
			DevMenuWidget->InitializeMenuTitle();
			
			i++;
		}
	}
}

void UDEPRECATED_DevMasterMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Assign each dev menu to this manager
	TArray<UWidget*> ChildWidgets = MenuSwitcher->GetAllChildren();
	
	int32 i = 0;
	for (UWidget* ChildWidget : ChildWidgets)
	{
		UDevMenuWidget* DevMenuWidget = Cast<UDevMenuWidget>(ChildWidget);
		if (DevMenuWidget)
		{			
			DevMenuWidget->AssignMenuManager(this);
			DevMenuWidget->MenuIndex = i;

			DevMenuWidget->RefreshCommandList();
			
			i++;
		}
	}

	Stack.Empty(1);

	Stack.Add(MenuSwitcher->GetWidgetAtIndex(0));
}
