// Copyright Void Interactive, 2021

#pragma once

#include "DevMasterMenuWidgetBase.h"
#include "DevMasterMenuWidget_INI.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class DEVMENU_API UDevMasterMenuWidget_INI : public UDevMasterMenuWidgetBase
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
};
