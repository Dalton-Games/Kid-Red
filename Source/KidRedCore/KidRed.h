// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"

#include "Blueprint/UserWidget.h"

#include "GameFramework/HUD.h"

#include "Animation/AnimInstance.h"

#include "Kismet/KismetMathLibrary.h"

#include "Net/UnrealNetwork.h"

#include "Engine/DataTable.h"

#include "KidRedEnums.h"
#include "KidRedStructs.h"
#include "KidRedDefines.h"

#include "KidRedPawn.h"
#include "KidRedCharacter.h"
#include "KidRedAICharacter.h"
#include "KidRedPlayerController.h"
#include "KidRedAIController.h"
#include "KidRedPlayerState.h"
#include "KidRedAnimInstance.h"
#include "KidRedSkeletalMeshComponent.h"
#include "KidRedSpectatorPawn.h"
#include "KidRedAssetManager.h"
#include "KidRedCameraShake.h"
#include "KidRedGameInstance.h"
#include "KidRedGameModeBase.h"
#include "KidRedGameStateBase.h"
#include "KidRedGameSession.h"
#include "KidRedGameSingleton.h"
#include "KidRedLevelScriptBase.h"
#include "KidRedFunctionLibrary.h"
#include "KidRedGameplayStatics.h"

#include "KidRedUI/Public/KidRedHUD.h"

#if !UE_BUILD_SHIPPING
#include "DrawDebugHelpers.h"
#include "Debug/Public/Log.h"
#endif
