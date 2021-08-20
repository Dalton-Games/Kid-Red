// Copyright 2020 Ali El Saleh. All Rights Reserved.

#pragma once

#define TICK_RATE_5 0.2f
#define TICK_RATE_10 0.01f
#define TICK_RATE_20 0.05f
#define TICK_RATE_30 0.0333333333333333f
#define TICK_RATE_60 0.0166666666666667f
#define TICK_RATE_120 0.0083333333333333f

// Checks to see if an actor is in a particular game mode. Destroy if not in specified game mode
#define GAMEMODE_CHECK(GameModeClass, GameStateClass, bDestroy) \
GameModeClass* GM = Cast<GameModeClass>(UGameplayStatics::GetGameMode(this)); \
GameStateClass* GS = Cast<GameStateClass>(UGameplayStatics::GetGameState(this)); \
if (!GM && !GS)\
{\
if (bDestroy) \
	Destroy();\
\
return;\
}\

#define EDITOR_ISGAMEVIEW() \
FEditorViewportClient* Client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();\
if (Client && Client->IsInGameView())\
{\
return;\
};\

#define DESTROY_COMPONENT(Component)\
if (IsValid(Component))\
{\
Component->DestroyComponent();\
Component = nullptr;\
}\

#define RETURN_NOWORLD if (!GetWorld() || (GetWorld() && GetWorld()->bIsTearingDown)) return;
#define RETURN_NOWORLD_i if (!GetWorld() || (GetWorld() && GetWorld()->bIsTearingDown)) return 0;
#define RETURN_NOWORLD_b if (!GetWorld() || (GetWorld() && GetWorld()->bIsTearingDown)) return false;
#define RETURN_NOWORLD_s if (!GetWorld() || (GetWorld() && GetWorld()->bIsTearingDown)) return "";

#define HIDE_ACTOR_CATEGORIES HideCategories = ("Rendering", "Replication", "Collision", "Input", "Actor", "LOD", "Cooking")\
