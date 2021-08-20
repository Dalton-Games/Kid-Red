// Copyright 2020 Ali El Saleh. All Rights Reserved.

#include "KidRedEditor.h"

DEFINE_LOG_CATEGORY(LogKidRedEditor);

#define LOCTEXT_NAMESPACE "FKidRedEditor"

void FKidRedEditor::StartupModule()
{
	UE_LOG(LogKidRedEditor, Warning, TEXT("KidRed Editor module started"));
}

void FKidRedEditor::ShutdownModule()
{
	UE_LOG(LogKidRedEditor, Warning, TEXT("KidRed Editor module shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FKidRedEditor, KidRedEditor)
