// Copyright Void Interactive, 2021

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/Widget.h"
#include "DevMenuFunctionLibrary.generated.h"

UCLASS(NotBlueprintable)
class DEVMENU_API UDevMenuFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Dev Menu Function Library")
	static UDevMasterMenuWidgetBase* GetMasterDevMenu();
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu Function Library")
	static bool IsDevMenuOpen();
	
	UFUNCTION(BlueprintPure, Category = "Dev Menu Function Library")
	static FString DevMenuCommandsConfigDir();
	
	static FString DevMenuCommandsPluginConfigDir();
	
	static TArray<FString> GetAllSectionNamesFromINIFile(FString ConfigFilePath);
	static TArray<FString> GetSingleLineArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	static int32 GetIntegerFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	static float GetFloatFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	static bool GetBoolFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	static FVector GetVectorFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	static FVector2D GetVector2DFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	static FString GetStringFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	static TArray<FString> GetStringArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key);

	template<class WidgetClass>
	static void RemoveFromParentAndClear(TArray<WidgetClass*>& InWidgets);
};

template <class WidgetClass>
void UDevMenuFunctionLibrary::RemoveFromParentAndClear(TArray<WidgetClass*>& InWidgets)
{
	static_assert(TIsDerivedFrom<WidgetClass, UWidget>::Value, "WidgetClass must be derived from a UWidget class");

	if (InWidgets.Num() == 0)
		return;
	
	InWidgets.Remove(nullptr);
	
	for (WidgetClass* Widget : InWidgets)
	{
		if (IsValid(Widget))
			Widget->RemoveFromParent();
	}

	InWidgets.Empty();
}
