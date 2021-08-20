// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Components/Widget.h"
#include "TimerManager.h"
#include "UObject/UObjectIterator.h"
#include "EngineUtils.h"
#include "HelperFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EGameBuildConfiguration : uint8
{
	Unknown,
    Editor,
    Debug,
    Development,
    Test,
    Shipping,
    FinalRelease
};

/* 
*
*/
UCLASS()
class HELPERFUNCTIONLIB_API UHelperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void SetupTimeline(UObject* Object, FTimeline& InTimeline, UCurveFloat* InCurveFloat, bool bLooping, float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName = NAME_None);
	static void SetupTimeline(UObject* Object, FTimeline& InTimeline, UCurveLinearColor* InCurveLinearColor, bool bLooping, float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName = NAME_None);
	static void SetupTimeline(UObject* Object, FTimeline& InTimeline, UCurveVector* InCurveVector, bool bLooping, float InPlaybackSpeed, const FName& TimelineCallbackFuncName, const FName& TimelineFinishedCallbackFuncName = NAME_None);

	template<class UserClass>
	static void StartTimerForCallback(FTimerHandle& TimerHandle, UserClass* Object, typename FTimerDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr CallbackFunc, float Rate, bool bLoop = false, bool bAdjustForTimeDilation = false, float FirstDelay = -1.0f);
	
	template<class UserClass>
	static void StartTimerForCallback(UserClass* Object, typename FTimerDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr CallbackFunc, float Rate, bool bLoop = false, bool bAdjustForTimeDilation = false, float FirstDelay = -1.0f);
	
	template<class UserClass>
	static void StartTimerForCallback(UserClass* Object, typename FTimerDelegate::TUObjectMethodDelegate_Const<UserClass>::FMethodPtr CallbackFunc, float Rate, bool bLoop = false, bool bAdjustForTimeDilation = false, float FirstDelay = -1.0f);
	
	// Make your callback function a UFUNCTION(), otherwise it will not work
	template <typename UObjectTemplate, typename... VarTypes>
    static void StartTimerForCallback_Args(FTimerHandle& TimerHandle, UObjectTemplate* Object, const FName& CallbackFuncName, float Rate, bool bLoop, bool bAdjustForTimeDilation, float FirstDelay, VarTypes... Vars);

	// Make your callback function a UFUNCTION(), otherwise it will not work
	template <typename UObjectTemplate, typename... VarTypes>
	static void StartTimerForCallback_Args(UObjectTemplate* Object, const FName& CallbackFuncName, float Rate, bool bLoop, bool bAdjustForTimeDilation, float FirstDelay, VarTypes... Vars);

	static void StopCallbackTimer(UObject* Object, FTimerHandle& TimerHandle);
	
	template<class ObjectClass>
	static TArray<ObjectClass*> GetObjectsOfClass();

	template<class ActorClass>
    static TArray<ActorClass*> GetActorsOfClass(UWorld* WorldContext);

	template<class WidgetClass>
	static void RemoveFromParentAndClear(TArray<WidgetClass*>& InWidgets);

	template<typename T>
	static TArray<TArray<T>> SplitArray(const TArray<T>& Array, int32 Chunks);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Utilities")
	static bool AnyTrue(UPARAM(ref) TArray<bool>& BoolArray);
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Utilities")
	static bool AnyFalse(UPARAM(ref) TArray<bool>& BoolArray);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Utilities")
	static bool AllTrue(UPARAM(ref) TArray<bool>& BoolArray);
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Utilities")
    static bool AllFalse(UPARAM(ref) TArray<bool>& BoolArray);
	
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | Utilities", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext", UnsafeDuringActorConstruction = "true", DeterminesOutputType = "BlueprintClass"))
    static UObject* CreateBlueprintObject(UObject* WorldContext, TSubclassOf<UObject> BlueprintClass, UObject* Outer);
	
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | Utilities", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext", UnsafeDuringActorConstruction = "true", DeterminesOutputType = "BlueprintClasses"))
    static TArray<UObject*> CreateBlueprintObjects(UObject* WorldContext, UPARAM(ref) TArray<TSubclassOf<UObject>>& BlueprintClasses, UObject* Outer);
	
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | Utilities", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext", UnsafeDuringActorConstruction = "true", DeterminesOutputType = "BlueprintClass"))
	static AActor* SpawnBlueprintActor(UObject* WorldContext, TSubclassOf<AActor> BlueprintClass, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | Utilities", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext", UnsafeDuringActorConstruction = "true", DeterminesOutputType = "BlueprintClasses"))
    static TArray<AActor*> SpawnBlueprintActors(UObject* WorldContext, UPARAM(ref) TArray<TSubclassOf<AActor>>& BlueprintClasses, const TArray<FTransform>& Transforms);

	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | Utilities", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext", UnsafeDuringActorConstruction = "true", DeterminesOutputType = "BlueprintClasses"))
    static TArray<AActor*> SpawnBlueprintActors_OneTransform(UObject* WorldContext, UPARAM(ref) TArray<TSubclassOf<AActor>>& BlueprintClasses, const FTransform& Transform);

	// Removes all widgets in array from parent and empties when done
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | Widget")
	static void RemoveFromParentAndClear(UPARAM(ref) TArray<UWidget*>& InWidgets);
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static TArray<FString> GetAllSectionNamesFromINIFile(FString ConfigFilePath);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static TArray<FString> GetSingleLineArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static int32 GetIntegerFromINIFile(FString ConfigFilePath, FString Section, FString Key);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static float GetFloatFromINIFile(FString ConfigFilePath, FString Section, FString Key);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static bool GetBoolFromINIFile(FString ConfigFilePath, FString Section, FString Key);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
    static FVector GetVectorFromINIFile(FString ConfigFilePath, FString Section, FString Key);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
    static FVector2D GetVector2DFromINIFile(FString ConfigFilePath, FString Section, FString Key);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static FString GetStringFromINIFile(FString ConfigFilePath, FString Section, FString Key);
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
    static TArray<FString> GetStringArrayFromINIFile(FString ConfigFilePath, FString Section, FString Key);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Widget", DisplayName = "GetWidgetSize (Absolute)")
	static FVector2D GetWidgetSize_Absolute(UWidget* InWidget);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Widget", DisplayName = "GetWidgetSize (Local)")
    static FVector2D GetWidgetSize_Local(UWidget* InWidget);

	UFUNCTION(BlueprintPure, Category = "Function Library | Widget", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
	static FVector2D GetViewportPositionOfWidget(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget, FVector2D InCoordinates = FVector2D(0.0f, 0.0f));
	
	UFUNCTION(BlueprintPure, Category = "Function Library | Widget", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
    static FVector2D GetPixelPositionOfWidget(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget, FVector2D InCoordinates = FVector2D(0.0f, 0.0f));
	
	UFUNCTION(BlueprintPure, Category = "Function Library | Widget", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
    static FVector2D GetViewportPositionOfWidgetCenter(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget);
	
	UFUNCTION(BlueprintPure, Category = "Function Library | Widget", meta = (DefaultToSelf = "WorldContext", HidePin = "WorldContext"))
    static FVector2D GetPixelPositionOfWidgetCenter(UObject* WorldContext, UWidget* InParentWidget, UWidget* InWidget);
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static FString GetProjectName();
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static FString GetProjectID();

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static FString GetProjectVersion();
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static EGameBuildConfiguration GetBuildConfiguration();
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static FString GetBuildConfigurationAsString();

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Level")
	static FName GetCurrentLevelName(UObject* Context);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Level")
	static ALevelScriptActor* GetLevelScript(UObject* Context);
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Level")
	static TArray<USoundBase*> GetAllSoundsInWorld();
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | System")
	static TArray<UClass*> GetAllClassesInMemory();
	
	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Utilities", meta = (DeterminesOutputType = "Object"))
	static UClass* GetClassFromObject(UObject* Object);

	UFUNCTION(BlueprintPure, Category = "Helper Function Library | Utilities", meta = (DeterminesOutputType = "Class"))
	static UObject* GetClassDefaultObject(UClass* Class);
	
	// Process name is the exe name (with the .exe included)
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | System")
	static int32 GetRunningProcessID_Windows(FString ProcessName);

	// Process name is the exe name (with the .exe included)
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | System")
	static bool IsProcessRunning_Windows(FString ProcessName);

	// DLL name is the module name (with the .dll included)
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | System")
	static bool IsDLLLoaded_Windows(FString DLLName);

	// Process name is the exe name (with the .exe included). DLL name is the module name (with the .dll included)
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | System")
	static bool IsDLLLoadedInProcess_Windows(FString ProcessName, FString DLLName);
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | System")
	static bool DoesProcessWindowTitleExist_Windows(FString ProcessWindowTitle);
	UFUNCTION(BlueprintCallable, Category = "Helper Function Library | System")
	static bool DoesProcessWindowTitleContain_Windows(FString ProcessWindowTitleSubstring);
};

template <class UserClass>
void UHelperFunctionLibrary::StartTimerForCallback(FTimerHandle& TimerHandle, UserClass* Object, typename FTimerDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr CallbackFunc, float Rate, bool bLoop, bool bAdjustForTimeDilation, float FirstDelay)
{
	if (Object)
	{
		if (Object->GetWorld())
		{
			Object->GetWorld()->GetTimerManager().SetTimer(TimerHandle, Object, CallbackFunc, Rate * (bAdjustForTimeDilation ? UGameplayStatics::GetGlobalTimeDilation(Object) : 1.0f), bLoop, FirstDelay);
		}
	}
}

template <class UserClass>
void UHelperFunctionLibrary::StartTimerForCallback(UserClass* Object, typename FTimerDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr CallbackFunc, float Rate, bool bLoop, bool bAdjustForTimeDilation, float FirstDelay)
{
	if (Object)
	{
		if (Object->GetWorld())
		{
			FTimerHandle TimerHandle;
			Object->GetWorld()->GetTimerManager().SetTimer(TimerHandle, Object, CallbackFunc, Rate * (bAdjustForTimeDilation ? UGameplayStatics::GetGlobalTimeDilation(Object) : 1.0f), bLoop, FirstDelay);
		}
	}
}

template <class UserClass>
void UHelperFunctionLibrary::StartTimerForCallback(UserClass* Object, typename FTimerDelegate::TUObjectMethodDelegate_Const<UserClass>::FMethodPtr CallbackFunc, float Rate, bool bLoop, bool bAdjustForTimeDilation, float FirstDelay)
{
	if (Object)
	{
		if (Object->GetWorld())
		{
			FTimerHandle TimerHandle;
			Object->GetWorld()->GetTimerManager().SetTimer(TimerHandle, Object, CallbackFunc, Rate * (bAdjustForTimeDilation ? UGameplayStatics::GetGlobalTimeDilation(Object) : 1.0f), bLoop, FirstDelay);
		}
	}
}

template <typename UObjectTemplate, typename ... VarTypes>
void UHelperFunctionLibrary::StartTimerForCallback_Args(FTimerHandle& TimerHandle, UObjectTemplate* Object, const FName& CallbackFuncName, float Rate, bool bLoop, bool bAdjustForTimeDilation, float FirstDelay, VarTypes... Vars)
{
	if (Object)
	{
		if (Object->GetWorld())
		{
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(Object, CallbackFuncName, Vars...);
			Object->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Rate * (bAdjustForTimeDilation ? UGameplayStatics::GetGlobalTimeDilation(Object) : 1.0f), bLoop, FirstDelay);
		}
	}
}

template <typename UObjectTemplate, typename ... VarTypes>
void UHelperFunctionLibrary::StartTimerForCallback_Args(UObjectTemplate* Object, const FName& CallbackFuncName, float Rate, bool bLoop, bool bAdjustForTimeDilation, float FirstDelay, VarTypes... Vars)
{
	if (Object)
	{
		if (Object->GetWorld())
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(Object, CallbackFuncName, Vars...);
			Object->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Rate * (bAdjustForTimeDilation ? UGameplayStatics::GetGlobalTimeDilation(Object) : 1.0f), bLoop, FirstDelay);
		}
	}
}

template <class ObjectClass>
TArray<ObjectClass*> UHelperFunctionLibrary::GetObjectsOfClass()
{
	static_assert(TIsDerivedFrom<ObjectClass, UObject>::Value, "ObjectClass must be derived from a UObject class");
	
	TArray<ObjectClass*> Objects;

	for (TObjectIterator<ObjectClass> It; It; ++It)
	{
		Objects.Add(*It);
	}

	return Objects;
}

template <class ActorClass>
TArray<ActorClass*> UHelperFunctionLibrary::GetActorsOfClass(UWorld* WorldContext)
{
	static_assert(TIsDerivedFrom<ActorClass, AActor>::Value, "ActorClass must be derived from an AActor class");

	if (!WorldContext)
		return TArray<ActorClass*>();
	
	TArray<ActorClass*> Actors;

	for (TActorIterator<ActorClass> It(WorldContext); It; ++It)
	{
		Actors.Add(*It);
	}

	return Actors;	
}

template <class WidgetClass>
void UHelperFunctionLibrary::RemoveFromParentAndClear(TArray<WidgetClass*>& InWidgets)
{
	static_assert(TIsDerivedFrom<WidgetClass, UWidget>::Value, "WidgetClass must be derived from a UWidget class");

	for (WidgetClass* Widget : InWidgets)
	{
		Widget->RemoveFromParent();
	}

	InWidgets.Empty();
}

template <typename T>
TArray<TArray<T>> UHelperFunctionLibrary::SplitArray(const TArray<T>& Array, int32 Chunks)
{
	if (Chunks <= 1)
		return TArray<TArray<int32>>();

	const int32 ArraySize = Array.Num();
	const int32 ElementsPerChunk = ArraySize/Chunks;
	const int32 LeftOverElements = ArraySize%Chunks;

	TArray<TArray<T>> ArrayChunks;
	ArrayChunks.Reserve(Chunks);
	
	TArray<int32> ElementsToAdd;
	ElementsToAdd.Reserve(ElementsPerChunk);
	
	for (int32 i = 0; i < Chunks; i++)
	{
		for (int32 j = i*ElementsPerChunk; j < ElementsPerChunk*(i+1); ++j)
		{
			ElementsToAdd.Add(j);
		}

		ArrayChunks.Add(ElementsToAdd);
		ElementsToAdd.Empty(ElementsPerChunk);
	}

	ElementsToAdd.Empty(ElementsPerChunk);

	for (int32 i = ArraySize-LeftOverElements; i < ArraySize; ++i)
	{
		ElementsToAdd.Add(i);
	}

	if (ElementsToAdd.Num() > 0)
		ArrayChunks.Add(ElementsToAdd);

	return Chunks;
}
