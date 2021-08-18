// Copyright Void Interactive, 2021

#include "DevMenuFunctionLibrary.h"

#include "Misc/ConfigCacheIni.h"
#include "Misc/Paths.h"

#include "Modules/ModuleManager.h"

#include "UObject/UObjectIterator.h"

UDevMasterMenuWidgetBase* UDevMenuFunctionLibrary::GetMasterDevMenu()
{
	return UDevMenu::Get()->DevMasterMenu;
}

bool UDevMenuFunctionLibrary::IsDevMenuOpen()
{
	for (const TObjectIterator<UDevMenu> It; It;)
		return (*It)->IsOpen();

	return false;
}

FString UDevMenuFunctionLibrary::DevMenuCommandsConfigDir()
{
	return FPaths::ProjectConfigDir() + "DevMenuCommands.ini";
}

FString UDevMenuFunctionLibrary::DevMenuCommandsPluginConfigDir()
{
	// Find out where we are loaded in (engine or project)
	FModuleStatus ModuleStatus;
	FModuleManager::Get().QueryModule("DevMenu", ModuleStatus);
	FString ModulePath = ModuleStatus.FilePath;
	
	const FString SearchString = "DevMenu/";
	const int32 ChopIndex = ModulePath.Find(SearchString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	ModulePath.RemoveAt(ChopIndex + SearchString.Len(), ModulePath.Len() - (ChopIndex - SearchString.Len()));
	
	return ModulePath + "Resources/DevMenuCommands-Template.ini";
}

TArray<FString> UDevMenuFunctionLibrary::GetAllSectionNamesFromINIFile(FString ConfigFilePath)
{
	TArray<FString> SectionNames;
	GConfig->GetSectionNames(ConfigFilePath, SectionNames);
	
	return SectionNames;
}

TArray<FString> UDevMenuFunctionLibrary::GetSingleLineArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	TArray<FString> StringArray;
	GConfig->GetSingleLineArray(*Section, *Key, StringArray, ConfigFilePath);

	for (int32 i = 0; i < StringArray.Num(); i++)
	{
		StringArray[i].RemoveFromEnd(TEXT(","));
	}

	return StringArray;
}

int32 UDevMenuFunctionLibrary::GetIntegerFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	int32 ConfigInt;
	GConfig->GetInt(*Section, *Key, ConfigInt, ConfigFilePath);
	
	return ConfigInt;
}

float UDevMenuFunctionLibrary::GetFloatFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	float ConfigFloat;
	GConfig->GetFloat(*Section, *Key, ConfigFloat, ConfigFilePath);

	return ConfigFloat;
}

bool UDevMenuFunctionLibrary::GetBoolFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	bool ConfigBool;
	GConfig->GetBool(*Section, *Key, ConfigBool, ConfigFilePath);

	return ConfigBool;
}

FVector UDevMenuFunctionLibrary::GetVectorFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	FVector ConfigVector;
	GConfig->GetVector(*Section, *Key, ConfigVector, ConfigFilePath);

	return ConfigVector;
}

FVector2D UDevMenuFunctionLibrary::GetVector2DFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	FVector2D ConfigVector;
	GConfig->GetVector2D(*Section, *Key, ConfigVector, ConfigFilePath);

	return ConfigVector;
}

FString UDevMenuFunctionLibrary::GetStringFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	FString ConfigString;
	GConfig->GetString(*Section, *Key, ConfigString, ConfigFilePath);
	
	return ConfigString;
}

TArray<FString> UDevMenuFunctionLibrary::GetStringArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	TArray<FString> ConfigStringArray;
	GConfig->GetArray(*Section, *Key, ConfigStringArray, ConfigFilePath);

	for (int32 i = 0; i < ConfigStringArray.Num(); i++)
	{
		ConfigStringArray[i].RemoveFromEnd(TEXT(","));
	}

	return ConfigStringArray;
}
