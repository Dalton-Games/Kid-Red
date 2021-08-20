// Copyright 2020 Ali El Saleh. All Rights Reserved.

#include "KidRedWeapons.h"

DEFINE_LOG_CATEGORY(LogKidRedWeapons);

#define LOCTEXT_NAMESPACE "FKidRedWeapons"

void FKidRedWeapons::StartupModule()
{
	UE_LOG(LogKidRedWeapons, Warning, TEXT("KidRed Weapons module started"));
}

void FKidRedWeapons::ShutdownModule()
{
	UE_LOG(LogKidRedWeapons, Warning, TEXT("KidRed Weapons module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKidRedWeapons, KidRedWeapons)
