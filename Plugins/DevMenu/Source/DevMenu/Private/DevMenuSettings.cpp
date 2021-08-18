// Copyright Ali El Saleh 2020

#include "DevMenuSettings.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

UDevMenuSettings::UDevMenuSettings()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> W_DevMasterMenu(TEXT("WidgetBlueprint'/DevMenu/W_DevMasterMenu.W_DevMasterMenu_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> W_DevMenuBase(TEXT("WidgetBlueprint'/DevMenu/W_DevMenuBase.W_DevMenuBase_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> W_DevCommandBase(TEXT("WidgetBlueprint'/DevMenu/W_DevCommandBase.W_DevCommandBase_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> W_DevCommand_ArrayBase(TEXT("WidgetBlueprint'/DevMenu/W_DevCommand_ArrayBase.W_DevCommand_ArrayBase_C'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> W_DevCommands_DividerBase(TEXT("WidgetBlueprint'/DevMenu/W_DevCommands_DividerBase.W_DevCommands_DividerBase_C'"));

	DevMasterMenuWidgetClass = W_DevMasterMenu.Class;
	DevMenuWidgetClass = W_DevMenuBase.Class;
	DevCommandWidgetClass = W_DevCommandBase.Class;
	DevCommandArrayWidgetClass = W_DevCommand_ArrayBase.Class;
	DevCommandDividerWidgetClass = W_DevCommands_DividerBase.Class;

	bEnabled = true;

	DevMenuKeys.Empty();
	DevMenuKeys.Add(EKeys::Backslash);
	DevMenuKeys.Add(EKeys::NumPadOne);

	NextCommandKeys.Empty();
	NextCommandKeys.Add(EKeys::Down);
	NextCommandKeys.Add(EKeys::Gamepad_DPad_Down);
	
	PreviousCommandKeys.Empty();
	PreviousCommandKeys.Add(EKeys::Up);
	PreviousCommandKeys.Add(EKeys::Gamepad_DPad_Up);
	
	SelectCommandKeys.Add(EKeys::Enter);
	SelectCommandKeys.Add(EKeys::Right);
	SelectCommandKeys.Add(EKeys::Gamepad_FaceButton_Bottom);

	PreviousMenuKeys.Add(EKeys::BackSpace);
	PreviousMenuKeys.Add(EKeys::Left);
	PreviousMenuKeys.Add(EKeys::Gamepad_FaceButton_Right);
}
