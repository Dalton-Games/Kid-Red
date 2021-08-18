// Copyright Void Interactive, 2021

#include "DevCommandWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"

#include "DevMenuScript.h"

#include "DevMenuWidget.h"
#include "DevCommandsDividerWidget.h"
#include "DevMasterMenuWidgetBase.h"

void UDevCommandWidget_Base::InitializeWidget_Implementation(UDevMenuWidget* InDevMenuOwner, UDevCommandsDividerWidget* InDividerWidgetOwner, UDevMenuScript* InCommandScript, const TArray<UDevMenuScript*>& InCommandScripts)
{
	DevMenuOwner = InDevMenuOwner;
	DividerWidgetOwner = InDividerWidgetOwner;
}

void UDevCommandWidget::InitializeWidget_Implementation(UDevMenuWidget* InDevMenuOwner, class UDevCommandsDividerWidget* InDividerWidgetOwner, UDevMenuScript* InCommandScript, const TArray<class UDevMenuScript*>& InCommandScripts)
{
	Super::InitializeWidget_Implementation(InDevMenuOwner, InDividerWidgetOwner, InCommandScript, InCommandScripts);

	CommandScript = InCommandScript;
	
	if (CommandScript)
	{
		CommandScript->NativeInitialize(InDevMenuOwner, this);

		if (CommandScript->GetCommandBehaviour() == EDevCommandBehaviour::DCB_Editable)
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(1);
			EditableValue_Text->SetText(CommandScript->GetStateValue());
			EditableValue_Text->WidgetStyle.SetForegroundColor(CommandScript->GetStateValueTextColor());
		}
		else if (CommandScript->GetCommandBehaviour() == EDevCommandBehaviour::DCB_MultiState)
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(2);
		}
		else if (CommandScript->GetCommandBehaviour() > EDevCommandBehaviour::DCB_Normal)
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(0);
			Value_Text->SetText(CommandScript->GetStateValue());
			Value_Text->SetColorAndOpacity(CommandScript->GetStateValueTextColor());
		}
		else
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(0);
			Value_Text->SetText(FText::FromString(""));
			Value_Text->SetColorAndOpacity(FLinearColor(FColor::White));
		}
	}
}

bool UDevCommandWidget::IsAllowedOnDevMenu() const
{
	if (CommandScript)
	{
		return CommandScript->IsAllowedOnDevMenu();
	}

	return false;
}

void UDevCommandWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CommandName_Text->SetText(DebugCommandName);
}

void UDevCommandWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Command_Button->OnClicked.AddDynamic(this, &UDevCommandWidget::OnCommandButtonClicked);
	Command_Button->OnHovered.AddDynamic(this, &UDevCommandWidget::OnCommandButtonHovered);
	Command_Button->OnUnhovered.AddDynamic(this, &UDevCommandWidget::OnCommandButtonUnhovered);
}

void UDevCommandWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CommandScript)
	{
		if (CommandScript->GetCommandBehaviour() == EDevCommandBehaviour::DCB_Editable)
		{
			EditableValue_Text->WidgetStyle.SetForegroundColor(CommandScript->GetStateValueTextColor());
		}
		else if (CommandScript->GetCommandBehaviour() == EDevCommandBehaviour::DCB_MultiState)
		{
		}
		else if (CommandScript->GetCommandBehaviour() > EDevCommandBehaviour::DCB_Normal)
		{
			if (CommandScript->UpdateValueTextEveryFrame())
				Value_Text->SetText(CommandScript->GetStateValue());
			
			Value_Text->SetColorAndOpacity(CommandScript->GetStateValueTextColor());
		}
	}
}

bool UDevCommandWidget::ExecuteCommand()
{
	if (CommandScript)
	{
		CommandScript->NativeExecuteScript();

		if (CommandScript->GetCommandBehaviour() == EDevCommandBehaviour::DCB_Editable)
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(1);
			EditableValue_Text->WidgetStyle.SetForegroundColor(CommandScript->GetStateValueTextColor());
		}
		else if (CommandScript->GetCommandBehaviour() == EDevCommandBehaviour::DCB_MultiState)
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(2);
		}
		else if (CommandScript->GetCommandBehaviour() > EDevCommandBehaviour::DCB_Normal)
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(0);
			Value_Text->SetText(CommandScript->GetStateValue());
			Value_Text->SetColorAndOpacity(CommandScript->GetStateValueTextColor());
		}
		else
		{
			ValueWidgetSwitcher->SetActiveWidgetIndex(0);
			Value_Text->SetText(FText::FromString(""));
			Value_Text->SetColorAndOpacity(FLinearColor(FColor::White));
		}

		if (CommandScript->ShouldCloseMenuOnExecute())
			DevMenuOwner->GetDevMenuManager()->HideMenu();

		return true;
	}

	return false;
}

void UDevCommandWidget_Array::InitializeWidget_Implementation(UDevMenuWidget* InDevMenuOwner, UDevCommandsDividerWidget* InDividerWidgetOwner, UDevMenuScript* InCommandScript, const TArray<class UDevMenuScript*>& InCommandScripts)
{
	Super::InitializeWidget_Implementation(InDevMenuOwner, InDividerWidgetOwner, InCommandScript, InCommandScripts);

	Command_Container->ClearChildren();
	CommandWidgets.Empty(InCommandScripts.Num());

	for (UDevMenuScript* Script : InCommandScripts)
	{
		UDevCommandWidget* DevCommand = CreateWidget<UDevCommandWidget>(GetOwningPlayer(), InDevMenuOwner->GetDevCommandWidgetClass());
		if (DevCommand)
		{
			DevCommand->SetPadding(FMargin(10.0f, 0.0f, 10.0f, 0.0f));
			DevCommand->SetCommandName(Script->GetCommandName());
			DevCommand->MenuIndex = InDevMenuOwner->GetNumCommands();
			DevCommand->SetVisibility(ESlateVisibility::Collapsed);

			DevCommand->InitializeWidget(InDevMenuOwner, InDividerWidgetOwner, Script, {});
						
			Command_Container->AddChild(DevCommand);
			CommandWidgets.Add(DevCommand);
		}
	}
	
	DevCommand_Divider->AssignDevCommands((TArray<UDevCommandWidget_Base*>)CommandWidgets);
}

bool UDevCommandWidget_Array::IsAllowedOnDevMenu() const
{
	for (UDevCommandWidget* Command : CommandWidgets)
	{
		if (Command && Command->IsAllowedOnDevMenu())
			return true;
	}

	return false;
}

void UDevCommandWidget_Array::SetGroupName(const FText& InGroupName)
{
	DevCommand_Divider->SetGroupName(InGroupName);
}

void UDevCommandWidget_Array::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDevCommandWidget_Array::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (UDevCommandWidget* Command : CommandWidgets)
	{
		if (Command && !DevCommand_Divider->IsCollapsed())
		{
			Command->SetVisibility(Command->IsAllowedOnDevMenu() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}
}

void UDevCommandWidget_Base::NativeConstruct()
{
	Super::NativeConstruct();

	#if UE_BUILD_SHIPPING
	SetVisibility(ESlateVisibility::Collapsed);
	RemoveFromParent();
	return;
	#endif

	HideSelectionArrow();
}

void UDevCommandWidget_Base::OnCommandButtonPressed()
{
	OnCommandButtonClicked();
}

void UDevCommandWidget_Base::OnCommandButtonSelected()
{
	OnCommandButtonHovered();
}

void UDevCommandWidget_Base::OnCommandButtonUnselected()
{
	OnCommandButtonUnhovered();
}

void UDevCommandWidget_Base::OnCommandButtonClicked()
{
	ExecuteCommand();

	if (DevMenuOwner)
	{
		if (DevMenuOwner->GetDevMenuManager() && DevMenuOwner->GetDevMenuManager()->IsOpen())
		{
			const bool bNotClickedLastTwoCommands = (MenuIndex != DevMenuOwner->GetNumCommands() - 1 && MenuIndex != DevMenuOwner->GetNumCommands() - 2);

			DevMenuOwner->GetDevMenuManager()->GetOwningPlayer()->SetInputMode(FInputModeGameAndUI().SetWidgetToFocus(DevMenuOwner->GetDevMenuManager()->TakeWidget()));

			if (DevMenuOwner->ShouldCloseMenuOnCommandSelection() && bNotClickedLastTwoCommands && !DevMenuOwner->IsSubMenu(MenuIndex))
				DevMenuOwner->GetDevMenuManager()->HideMenu();
		}
	}
}

void UDevCommandWidget_Base::OnCommandButtonHovered()
{
	DevMenuOwner->SelectCommand(MenuIndex);

	ShowSelectionArrow();
}

void UDevCommandWidget_Base::OnCommandButtonUnhovered()
{
	HideSelectionArrow();
}

void UDevCommandWidget::ShowSelectionArrow()
{
	SelectionArrow->SetVisibility(ESlateVisibility::Visible);
	SelectionArrow->SetColorAndOpacity(ArrowSelectedColor);
}

void UDevCommandWidget::HideSelectionArrow()
{
	SelectionArrow->SetVisibility(ESlateVisibility::Hidden);
	SelectionArrow->SetColorAndOpacity(ArrowDeselectedColor);
}

void UDevCommandWidget::SetCommandName(const FText& NewCommandName)
{
	DebugCommandName = NewCommandName;
	CommandName_Text->SetText(NewCommandName);
}

FText UDevCommandWidget::GetCommandName() const
{
	return CommandName_Text->GetText();
}
