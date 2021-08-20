// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedItems.h"

DEFINE_LOG_CATEGORY(LogKidRedItems);

#define LOCTEXT_NAMESPACE "FKidRedItems"

void FKidRedItems::StartupModule()
{
	UE_LOG(LogKidRedItems, Warning, TEXT("KidRed Items module started"));
}

void FKidRedItems::ShutdownModule()
{
	UE_LOG(LogKidRedItems, Warning, TEXT("KidRed Items module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKidRedItems, KidRedItems)
