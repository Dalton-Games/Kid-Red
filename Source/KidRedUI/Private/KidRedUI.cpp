// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedUI.h"

DEFINE_LOG_CATEGORY(LogKidRedUI);

#define LOCTEXT_NAMESPACE "FKidRedUI"

void FKidRedUI::StartupModule()
{
	UE_LOG(LogKidRedUI, Warning, TEXT("KidRed UI module started"));
}

void FKidRedUI::ShutdownModule()
{
	UE_LOG(LogKidRedUI, Warning, TEXT("KidRed UI module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKidRedUI, KidRedUI)
