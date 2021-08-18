// Copyright Void Interactive, 2021

#include "DevCommandsDividerWidget.h"

#include "Components/TextBlock.h"

#include "DevCommandWidget.h"

UDevCommandsDividerWidget::UDevCommandsDividerWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UDevCommandsDividerWidget::ToggleCommandVisibility()
{
	if (bIsSimpleDivider)
		return false;
	
	if (bIsCollapsed)
	{
		ExpandCommands();
		return true;
	}

	CollapseCommands();
	return false;
}

void UDevCommandsDividerWidget::ExpandCommands()
{
	// Remove all null elements
	DevCommandWidgets.Remove(nullptr);

	if (DevCommandWidgets.Num() > 0)
	{
		for (auto* CommandWidget : DevCommandWidgets)
		{
			CommandWidget->SetVisibility(CommandWidget->IsAllowedOnDevMenu() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}

	bIsCollapsed = false;
}

void UDevCommandsDividerWidget::CollapseCommands()
{
	// Remove all null elements
	DevCommandWidgets.Remove(nullptr);

	if (DevCommandWidgets.Num() > 0)
	{
		for (auto* CommandWidget : DevCommandWidgets)
		{
			CommandWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	bIsCollapsed = true;
}

void UDevCommandsDividerWidget::AssignDevCommands(const TArray<UDevCommandWidget_Base*>& InDevCommandWidgets)
{
	DevCommandWidgets = InDevCommandWidgets;
}

void UDevCommandsDividerWidget::AddDevCommands(const TArray<UDevCommandWidget_Base*>& InDevCommandWidgets)
{
	DevCommandWidgets += InDevCommandWidgets;
}

void UDevCommandsDividerWidget::SetGroupName(const FText& NewGroupName)
{
	GroupName_Text->SetText(NewGroupName);
}

void UDevCommandsDividerWidget::SetIsEmpty(const bool bIsEmpty)
{
	bIsSimpleDivider = bIsEmpty;
}

bool UDevCommandsDividerWidget::AreAllDevCommandsCollapsed() const
{
	if (bIsSimpleDivider || bIsCollapsed)
		return false;

	bool bAllCollapsed = true;
	for (UDevCommandWidget_Base* Widget : DevCommandWidgets)
	{
		if (Widget->GetVisibility() != ESlateVisibility::Collapsed)
		{
			bAllCollapsed = false;
			break;
		}
	}

	return bAllCollapsed;
}
