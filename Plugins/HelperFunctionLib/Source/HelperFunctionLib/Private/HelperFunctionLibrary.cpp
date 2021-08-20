// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelperFunctionLibrary.h"

#include "GeneralProjectSettings.h"
#include "Blueprint/SlateBlueprintLibrary.h"

#include "Engine/World.h"

#include "Sound/SoundBase.h"

#include "Curves/CurveFloat.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveVector.h"

#include "Misc/ConfigCacheIni.h"

#if PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN // <-- Fixes compiler error: winioctl.h(10265): [C4668] '_WIN32_WINNT_WIN10_TH2' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'

#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <string>
#endif

void UHelperFunctionLibrary::SetupTimeline(UObject* Object, FTimeline& InTimeline, UCurveFloat* InCurveFloat, const bool bLooping, const float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName)
{
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(Object, TimelineCallbackFuncName);

	if (InCurveFloat)
	{
		InTimeline.AddInterpFloat(InCurveFloat, TimelineCallback);
		InTimeline.SetTimelineLength(InCurveFloat->FloatCurve.Keys[InCurveFloat->FloatCurve.Keys.Num() - 1].Time);
	}

	if (TimelineCallbackFuncName != NAME_None)
	{
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(Object, TimelineFinishedCallbackFuncName);
		InTimeline.SetLooping(bLooping);
		InTimeline.SetPlayRate(InPlaybackSpeed);
		InTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		InTimeline.SetTimelineLengthMode(TL_TimelineLength);
	}
}

void UHelperFunctionLibrary::SetupTimeline(UObject* Object, FTimeline& InTimeline, UCurveLinearColor* InCurveLinearColor, const bool bLooping, const float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName)
{
	FOnTimelineLinearColor TimelineCallback;
	TimelineCallback.BindUFunction(Object, TimelineCallbackFuncName);

	if (InCurveLinearColor)
	{
		InTimeline.AddInterpLinearColor(InCurveLinearColor, TimelineCallback);
		InTimeline.SetTimelineLength(InCurveLinearColor->FloatCurves->Keys[InCurveLinearColor->FloatCurves->GetNumKeys() - 1].Time);
	}

	if (TimelineCallbackFuncName != NAME_None)
	{
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(Object, TimelineFinishedCallbackFuncName);
		InTimeline.SetLooping(bLooping);
		InTimeline.SetPlayRate(InPlaybackSpeed);
		InTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		InTimeline.SetTimelineLengthMode(TL_TimelineLength);
	}
}

void UHelperFunctionLibrary::SetupTimeline(UObject* Object, FTimeline& InTimeline, UCurveVector* InCurveVector, const bool bLooping, const float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName)
{
	FOnTimelineVector TimelineCallback;
	TimelineCallback.BindUFunction(Object, TimelineCallbackFuncName);

	if (InCurveVector)
	{
		InTimeline.AddInterpVector(InCurveVector, TimelineCallback);
		InTimeline.SetTimelineLength(InCurveVector->FloatCurves->Keys[InCurveVector->FloatCurves->GetNumKeys() - 1].Time);
	}

	if (TimelineCallbackFuncName != NAME_None)
	{
		FOnTimelineEvent TimelineFinishedCallback;
		TimelineFinishedCallback.BindUFunction(Object, TimelineFinishedCallbackFuncName);
		InTimeline.SetLooping(bLooping);
		InTimeline.SetPlayRate(InPlaybackSpeed);
		InTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		InTimeline.SetTimelineLengthMode(TL_TimelineLength);
	}
}

void UHelperFunctionLibrary::StopCallbackTimer(UObject* Object, FTimerHandle& TimerHandle)
{
	if (Object)
	{
		if (Object->GetWorld())
		{
			Object->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	}
}

bool UHelperFunctionLibrary::AnyTrue(TArray<bool>& BoolArray)
{
	for (int32 i = 0; i < BoolArray.Num(); i++)
	{
		if (BoolArray[i])
		{
			return true;
		}
	}

	return false;
}

bool UHelperFunctionLibrary::AnyFalse(TArray<bool>& BoolArray)
{
	for (int32 i = 0; i < BoolArray.Num(); i++)
	{
		if (!BoolArray[i])
		{
			return true;
		}
	}

	return false;
}

bool UHelperFunctionLibrary::AllTrue(TArray<bool>& BoolArray)
{
	for (int32 i = 0; i < BoolArray.Num(); i++)
	{
		if (!BoolArray[i])
		{
			return false;
		}
	}

	return true;
}

bool UHelperFunctionLibrary::AllFalse(TArray<bool>& BoolArray)
{
	for (int32 i = 0; i < BoolArray.Num(); i++)
	{
		if (BoolArray[i])
		{
			return false;
		}
	}

	return true;
}

UObject* UHelperFunctionLibrary::CreateBlueprintObject(UObject* WorldContext, const TSubclassOf<UObject> BlueprintClass, UObject* Outer)
{
	if (!BlueprintClass || !WorldContext && !WorldContext->GetWorld())
		return nullptr;

	return NewObject<UObject>(Outer, BlueprintClass, BlueprintClass->GetFName(), RF_NoFlags, BlueprintClass.GetDefaultObject(), true);
}

TArray<UObject*> UHelperFunctionLibrary::CreateBlueprintObjects(UObject* WorldContext, TArray<TSubclassOf<UObject>>& BlueprintClasses, UObject* Outer)
{
	BlueprintClasses.RemoveAll([](const TSubclassOf<UObject> BlueprintClass)
	{
		return BlueprintClass == nullptr;
	});
	
	if (BlueprintClasses.Num() == 0 || !WorldContext && !WorldContext->GetWorld())
		return TArray<UObject*>();

	TArray<UObject*> ConstructedObjects;
	ConstructedObjects.Reserve(BlueprintClasses.Num()+1);
	
	for (TSubclassOf<UObject> BlueprintClass : BlueprintClasses)
	{
		ConstructedObjects.Add(NewObject<UObject>(Outer, BlueprintClass, BlueprintClass->GetFName(), RF_NoFlags, BlueprintClass.GetDefaultObject(), true));
	}

	return ConstructedObjects;
}

AActor* UHelperFunctionLibrary::SpawnBlueprintActor(UObject* WorldContext, const TSubclassOf<AActor> BlueprintClass, const FTransform& Transform)
{
	if (!BlueprintClass || !WorldContext && !WorldContext->GetWorld())
		return nullptr;

	return WorldContext->GetWorld()->SpawnActor<AActor>(BlueprintClass, Transform);
}

TArray<AActor*> UHelperFunctionLibrary::SpawnBlueprintActors(UObject* WorldContext, TArray<TSubclassOf<AActor>>& BlueprintClasses, const TArray<FTransform>& Transforms)
{
	BlueprintClasses.RemoveAll([](const TSubclassOf<UObject> BlueprintClass)
	{
	    return BlueprintClass == nullptr;
	});
	
	if (BlueprintClasses.Num() == 0 || !WorldContext && !WorldContext->GetWorld())
		return TArray<AActor*>();

	TArray<AActor*> SpawnedActors;
	SpawnedActors.Reserve(BlueprintClasses.Num()+1);

	int32 i = 0;
	for (TSubclassOf<UObject> BlueprintClass : BlueprintClasses)
	{
		SpawnedActors.Add(WorldContext->GetWorld()->SpawnActor<AActor>(BlueprintClass, Transforms.IsValidIndex(i) ? Transforms[i] : FTransform::Identity));
		i++;
	}

	return SpawnedActors;
}

TArray<AActor*> UHelperFunctionLibrary::SpawnBlueprintActors_OneTransform(UObject* WorldContext, TArray<TSubclassOf<AActor>>& BlueprintClasses, const FTransform& Transform)
{
	BlueprintClasses.RemoveAll([](const TSubclassOf<UObject> BlueprintClass)
	{
	    return BlueprintClass == nullptr;
	});
	
	if (BlueprintClasses.Num() == 0 || !WorldContext && !WorldContext->GetWorld())
		return TArray<AActor*>();

	TArray<AActor*> SpawnedActors;
	SpawnedActors.Reserve(BlueprintClasses.Num()+1);

	int32 i = 0;
	for (TSubclassOf<UObject> BlueprintClass : BlueprintClasses)
	{
		SpawnedActors.Add(WorldContext->GetWorld()->SpawnActor<AActor>(BlueprintClass, Transform));
		i++;
	}

	return SpawnedActors;
}

void UHelperFunctionLibrary::RemoveFromParentAndClear(TArray<UWidget*>& InWidgets)
{
	for (UWidget* Widget : InWidgets)
	{
		Widget->RemoveFromParent();
	}

	InWidgets.Empty();
}

TArray<FString> UHelperFunctionLibrary::GetAllSectionNamesFromINIFile(const FString ConfigFilePath)
{
	TArray<FString> SectionNames;
	GConfig->GetSectionNames(ConfigFilePath, SectionNames);
	
	return SectionNames;
}

TArray<FString> UHelperFunctionLibrary::GetSingleLineArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	TArray<FString> StringArray;
	GConfig->GetSingleLineArray(*Section, *Key, StringArray, ConfigFilePath);

	for (int32 i = 0; i < StringArray.Num(); i++)
	{
		StringArray[i].RemoveFromEnd(TEXT(","));
	}

	return StringArray;
}

int32 UHelperFunctionLibrary::GetIntegerFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	int32 ConfigInt;
	GConfig->GetInt(*Section, *Key, ConfigInt, ConfigFilePath);
	
	return ConfigInt;
}

float UHelperFunctionLibrary::GetFloatFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	float ConfigFloat;
	GConfig->GetFloat(*Section, *Key, ConfigFloat, ConfigFilePath);

	return ConfigFloat;
}

bool UHelperFunctionLibrary::GetBoolFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	bool ConfigBool;
	GConfig->GetBool(*Section, *Key, ConfigBool, ConfigFilePath);

	return ConfigBool;
}

FVector UHelperFunctionLibrary::GetVectorFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	FVector ConfigVector;
	GConfig->GetVector(*Section, *Key, ConfigVector, ConfigFilePath);

	return ConfigVector;
}

FVector2D UHelperFunctionLibrary::GetVector2DFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	FVector2D ConfigVector;
	GConfig->GetVector2D(*Section, *Key, ConfigVector, ConfigFilePath);

	return ConfigVector;
}

FString UHelperFunctionLibrary::GetStringFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	FString ConfigString;
	GConfig->GetString(*Section, *Key, ConfigString, ConfigFilePath);
	
	return ConfigString;
}

TArray<FString> UHelperFunctionLibrary::GetStringArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key)
{
	TArray<FString> ConfigStringArray;
	GConfig->GetArray(*Section, *Key, ConfigStringArray, ConfigFilePath);

	for (int32 i = 0; i < ConfigStringArray.Num(); i++)
	{
		ConfigStringArray[i].RemoveFromEnd(TEXT(","));
	}

	return ConfigStringArray;
}

FVector2D UHelperFunctionLibrary::GetWidgetSize_Absolute(UWidget* InWidget)
{
	if (InWidget)
	{
		return InWidget->GetCachedGeometry().GetAbsoluteSize();
	}

	return FVector2D::ZeroVector;
}

FVector2D UHelperFunctionLibrary::GetWidgetSize_Local(UWidget* InWidget)
{
	if (InWidget)
	{
		return InWidget->GetCachedGeometry().GetLocalSize();
	}

	return FVector2D::ZeroVector;
}

FVector2D UHelperFunctionLibrary::GetViewportPositionOfWidget(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget, const FVector2D InCoordinates)
{
	if (InWidget)
	{
		FVector2D ViewportPosition;
		FVector2D PixelPosition;
		USlateBlueprintLibrary::LocalToViewport(WorldContext, InParentWidget->GetTickSpaceGeometry(), InWidget->GetCachedGeometry().GetLocalPositionAtCoordinates(InCoordinates), PixelPosition, ViewportPosition);
		
		return ViewportPosition;
	}

	return FVector2D::ZeroVector;
}

FVector2D UHelperFunctionLibrary::GetPixelPositionOfWidget(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget, const FVector2D InCoordinates)
{
	if (InWidget)
	{
		FVector2D ViewportPosition;
		FVector2D PixelPosition;
		USlateBlueprintLibrary::LocalToViewport(WorldContext, InParentWidget->GetTickSpaceGeometry(), InWidget->GetCachedGeometry().GetLocalPositionAtCoordinates(InCoordinates), PixelPosition, ViewportPosition);
		
		return PixelPosition;
	}

	return FVector2D::ZeroVector;
}

FVector2D UHelperFunctionLibrary::GetViewportPositionOfWidgetCenter(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget)
{
	return GetViewportPositionOfWidget(WorldContext, InParentWidget, InWidget, {0.5f, 0.5f});
}

FVector2D UHelperFunctionLibrary::GetPixelPositionOfWidgetCenter(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget)
{
	return GetPixelPositionOfWidget(WorldContext, InParentWidget, InWidget, {0.5f, 0.5f});
}

FString UHelperFunctionLibrary::GetProjectName()
{
	if (const UGeneralProjectSettings* GeneralProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		return GeneralProjectSettings->ProjectName;
	}

	return "";
}

FString UHelperFunctionLibrary::GetProjectID()
{
	if (const UGeneralProjectSettings* GeneralProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		return GeneralProjectSettings->ProjectID.ToString();
	}

	return "";
}

FString UHelperFunctionLibrary::GetProjectVersion()
{
	if (const UGeneralProjectSettings* GeneralProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		return GeneralProjectSettings->ProjectVersion;
	}

	return "";
}

EGameBuildConfiguration UHelperFunctionLibrary::GetBuildConfiguration()
{
	#if WITH_EDITOR
		return EGameBuildConfiguration::Editor;
	#elif UE_BUILD_DEBUG
		return EGameBuildConfiguration::Debug;
	#elif UE_BUILD_DEVELOPMENT
		return EGameBuildConfiguration::Development;
	#elif UE_BUILD_TEST
		return EGameBuildConfiguration::Test;
	#elif UE_BUILD_SHIPPING
		return EGameBuildConfiguration::Shipping;
	#elif UE_BUILD_FINAL_RELEASE
		return EGameBuildConfiguration::FinalRelease;
	#else
		return EGameBuildConfiguration::Unknown;
	#endif
}

FString UHelperFunctionLibrary::GetBuildConfigurationAsString()
{
	switch (GetBuildConfiguration())
	{
		case EGameBuildConfiguration::Unknown:			return "Unknown";
		case EGameBuildConfiguration::Editor:			return "Editor";
		case EGameBuildConfiguration::Debug:			return "Debug";
		case EGameBuildConfiguration::Development:		return "Development";
		case EGameBuildConfiguration::Test:				return "Test";
		case EGameBuildConfiguration::Shipping:			return "Shipping";
		case EGameBuildConfiguration::FinalRelease:		return "Final Release";
		default:										return "Unknown";
	}
}

FName UHelperFunctionLibrary::GetCurrentLevelName(UObject* Context)
{
	if (!Context && !Context->GetWorld())
		return NAME_None;
	
	return *Context->GetWorld()->GetName();
}

TArray<USoundBase*> UHelperFunctionLibrary::GetAllSoundsInWorld()
{
	return GetObjectsOfClass<USoundBase>();
}

TArray<UClass*> UHelperFunctionLibrary::GetAllClassesInMemory()
{
	return GetObjectsOfClass<UClass>();
}

ALevelScriptActor* UHelperFunctionLibrary::GetLevelScript(UObject* Context)
{
	if (!Context && !Context->GetWorld())
		return nullptr;
	
	return Context->GetWorld()->GetLevelScriptActor(Context->GetWorld()->GetCurrentLevel());
}

UClass* UHelperFunctionLibrary::GetClassFromObject(UObject* Object)
{
	if (!Object)
		return nullptr;
	
	return Object->GetClass(); 
}

UObject* UHelperFunctionLibrary::GetClassDefaultObject(UClass* Class)
{
	if (!Class)
		return nullptr;

	return Class->GetDefaultObject();
}

#if PLATFORM_WINDOWS
int32 UHelperFunctionLibrary::GetRunningProcessID_Windows(const FString ProcessName)
{
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	int32 FoundProcessID = -1;

	const HANDLE SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(SnapshotHandle, &ProcessEntry))
	{
		while (Process32Next(SnapshotHandle, &ProcessEntry))
		{
			if (!_wcsicmp(ProcessEntry.szExeFile, *ProcessName))
			{
				FoundProcessID = ProcessEntry.th32ProcessID;
				break;
			}
		}
	}

	CloseHandle(SnapshotHandle);
	return FoundProcessID;
}

bool UHelperFunctionLibrary::IsProcessRunning_Windows(const FString ProcessName)
{
	bool bExists = false;
	PROCESSENTRY32 ProcessEntry;
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);

	const HANDLE SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(SnapshotHandle, &ProcessEntry))
	{
		while (Process32Next(SnapshotHandle, &ProcessEntry))
		{
			if (!_wcsicmp(ProcessEntry.szExeFile, *ProcessName))
			{
				bExists = true;
				break;
			}
		}
	}

	CloseHandle(SnapshotHandle);
	return bExists;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TArray<FString>* WindowTitles = reinterpret_cast<TArray<FString>*>(lParam);

	const int Length = GetWindowTextLengthA(hwnd);
	
	std::string WindowTitle(Length + 1, '\0');
	GetWindowTextA(hwnd, &WindowTitle[0], Length + 1);
	
	const FString WindowTitleString = WindowTitle.c_str();

	if (!WindowTitleString.IsEmpty())
		WindowTitles->Add(WindowTitleString);

	return true;
}

bool UHelperFunctionLibrary::DoesProcessWindowTitleExist_Windows(FString ProcessWindowTitle)
{
	TArray<FString> WindowTitles;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&WindowTitles));

	for (const FString& Title : WindowTitles)
	{
		if (Title == ProcessWindowTitle)
		{
			return true;
		}
	}
	
	return false;
}

bool UHelperFunctionLibrary::DoesProcessWindowTitleContain_Windows(FString ProcessWindowTitleSubstring)
{
	TArray<FString> WindowTitles;
	EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&WindowTitles));

	for (const FString& Title : WindowTitles)
	{
		if (Title.Contains(ProcessWindowTitleSubstring))
		{	
			return true;
		}
	}
	
	return false;
}


bool UHelperFunctionLibrary::IsDLLLoaded_Windows(const FString DLLName)
{
	HMODULE Modules[1024];
	DWORD ModulesNeeded;

	// Get a handle to this process
	const HANDLE Process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	if (!Process)
		return false;

	bool bExists = false;
	
	// Get a list of all the modules in this process
	if (EnumProcessModules(Process, Modules, sizeof(Modules), &ModulesNeeded))
	{
		for (uint32 i = 0; i < ModulesNeeded / sizeof(HMODULE); i++)
		{
			TCHAR ModuleName[MAX_PATH];

			// Get the name module
			GetModuleBaseName(Process, Modules[i], ModuleName, sizeof(ModuleName) / sizeof(TCHAR));

			if (FString(ModuleName) == DLLName)
			{
				// DLL has been found
				bExists = true;
				break;
			}
		}
	}
    
	// Release the handle to this process
	CloseHandle(Process);
	return bExists;
}

bool UHelperFunctionLibrary::IsDLLLoadedInProcess_Windows(const FString ProcessName, const FString DLLName)
{
	HMODULE Modules[1024];
	DWORD ModulesNeeded;
	const DWORD ProcessID = GetRunningProcessID_Windows(ProcessName);

	// Get a handle to the process.
	const HANDLE Process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessID);

	if (!Process)
		return false;

	bool bExists = false;

	// Get a list of all the modules in this process
	if (EnumProcessModules(Process, Modules, sizeof(Modules), &ModulesNeeded))
	{
		for (uint32 i = 0; i < (ModulesNeeded / sizeof(HMODULE)); i++ )
		{
			TCHAR ModuleName[MAX_PATH];

			// Get the name module
			GetModuleBaseName(Process, Modules[i], ModuleName, sizeof(ModuleName) / sizeof(TCHAR));

			if (FString(ModuleName) == DLLName)
			{
				bExists = true;
				break;
			}
		}
	}
    
	// Release the handle to the process.
	CloseHandle(Process);
	return bExists;
}
#endif

//todo: wrap in #PLATFORM_WINDOWS
