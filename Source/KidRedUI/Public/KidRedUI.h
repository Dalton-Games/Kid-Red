// Copyright 2020 Ali El Saleh. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "KidRedHUD.h"
#include "KidRedHUDWidget.h"
#include "KidRedCore/KidRedDefines.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogKidRedUI, All, All);
 
class FKidRedUI : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};