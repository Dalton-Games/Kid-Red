// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "DataRegistryTypes.h"
#include "Engine/DataTable.h"
#include "KidRedCore/KidRedDefines.h"

#if !UE_BUILD_SHIPPING
#include "DrawDebugHelpers.h"
#include "Debug/Public/Log.h"
#endif

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogKidRedItems, All, All);
 
class FKidRedItems : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};