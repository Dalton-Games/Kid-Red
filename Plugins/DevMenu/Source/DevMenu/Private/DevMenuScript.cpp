// Copyright Void Interactive, 2021

#include "DevMenuScript.h"

#include "DevMenuWidget.h"
#include "DevMasterMenuWidgetBase.h"

#include "Kismet/KismetSystemLibrary.h"

UDevMenuScript::UDevMenuScript(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDevMenuScript::NativeInitialize(UDevMenuWidget* InDevMenuOwner, UDevCommandWidget* InDevCommandWidgetOwner)
{
	DevMenuOwner = InDevMenuOwner;
	DevCommandWidgetOwner = InDevCommandWidgetOwner;
	
	World = GetWorld();

	Initialize(InDevMenuOwner, InDevCommandWidgetOwner);
}

void UDevMenuScript::Initialize_Implementation(UDevMenuWidget* InDevMenuOwner, class UDevCommandWidget* InDevCommandWidgetOwner)
{
}

void UDevMenuScript::NativeExecuteScript()
{
	ExecuteScript();
}

/*
void UDebugMenuScript::Tick(float DeltaTime)
{
}
	

bool UDebugMenuScript::IsTickable() const
{
	// FTickableObjectBase seems to call Tick() on objects that may be in the middle of being destroyed,
	// which will usually then crash since `this` is invalid.
	return IsValid(this) && !HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed | RF_ClassDefaultObject);
}

TStatId UDebugMenuScript::GetStatId() const
{
	return GetStatID();
}
*/

UWorld* UDevMenuScript::GetWorld() const
{
	if (HasAllFlags(RF_ClassDefaultObject))
		return nullptr;
	
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

bool UDevMenuScript::IsAllowedOnDevMenu_Implementation()
{
	return true;
}

bool UDevMenuScript::UpdateValueTextEveryFrame_Implementation()
{
	return false;
}

void UDevMenuScript::ExecuteScript_Implementation()
{
}

FText UDevMenuScript::GetStateValue_Implementation()
{
	return FText::FromString("");
}

FLinearColor UDevMenuScript::GetStateValueTextColor_Implementation()
{
	return FColor::White;
}

/////////////// Go Back /////////////// 
UDevMenuScript_GoBack::UDevMenuScript_GoBack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DebugCommandName = FText::FromString("Back...");
}

void UDevMenuScript_GoBack::ExecuteScript_Implementation()
{
	DevMenuOwner->GetDevMenuManager()->PreviousMenu();
}

/////////////// Open Debug Menu /////////////// 
UDevMenuScript_OpenDebugMenu::UDevMenuScript_OpenDebugMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDevMenuScript_OpenDebugMenu::SetMenuToOpen(UDevMenuWidget* InDevMenuToOpen)
{
	DevMenuToOpen = InDevMenuToOpen;
}

void UDevMenuScript_OpenDebugMenu::ExecuteScript_Implementation()
{
	DevMenuOwner->GetDevMenuManager()->OpenMenu(DevMenuToOpen);
}

/////////////// Exit Menu /////////////// 
UDevMenuScript_ExitMenu::UDevMenuScript_ExitMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DebugCommandName = FText::FromString("Exit Dev Menu");
}

void UDevMenuScript_ExitMenu::ExecuteScript_Implementation()
{
	DevMenuOwner->GetDevMenuManager()->HideMenu();
}

UDevMenuScript_ExitToMainDevMenu::UDevMenuScript_ExitToMainDevMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DebugCommandName = FText::FromString("Return to Main Menu");
}

void UDevMenuScript_ExitToMainDevMenu::ExecuteScript_Implementation()
{
	DevMenuOwner->GetDevMenuManager()->ResetStack();
}

/////////////// Console Command /////////////// 
UDevMenuScript_ConsoleCommand::UDevMenuScript_ConsoleCommand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DebugCommandName = FText::FromString("Console Command-0");
}

void UDevMenuScript_ConsoleCommand::ExecuteScript_Implementation()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, ConsoleCommand, DevMenuOwner->GetOwningPlayer());
}
