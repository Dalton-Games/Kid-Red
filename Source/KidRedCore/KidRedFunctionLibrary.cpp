// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedFunctionLibrary.h"

UWorld* UKidRedFunctionLibrary::GetWorldStatic(const bool bRequiresGameState)
{
	if (!GEngine)
		return nullptr;

	const TIndirectArray<FWorldContext> WorldContexts = GEngine->GetWorldContexts();
	
	for (FWorldContext World : WorldContexts)
	{
		if (bRequiresGameState)
		{
			if (World.World() && World.World()->GetGameState())
			{
				return World.World();
			}
		}
		else
		{
			if (World.World())
			{
				return World.World();
			}
		}
	}
	
	return nullptr;
}

float UKidRedFunctionLibrary::EaseAlpha(const float InAlpha, const TEnumAsByte<EEasingFunc::Type> EasingFunc, const float BlendExp, const int32 Steps)
{
	switch (EasingFunc)
	{
		case EEasingFunc::Step:					return FMath::InterpStep<float>(0.f, 1.f, InAlpha, Steps);
		case EEasingFunc::SinusoidalIn:			return FMath::InterpSinIn<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::SinusoidalOut:		return FMath::InterpSinOut<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::SinusoidalInOut:		return FMath::InterpSinInOut<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::EaseIn:				return FMath::InterpEaseIn<float>(0.f, 1.f, InAlpha, BlendExp);
		case EEasingFunc::EaseOut:				return FMath::InterpEaseOut<float>(0.f, 1.f, InAlpha, BlendExp);
		case EEasingFunc::EaseInOut:			return FMath::InterpEaseInOut<float>(0.f, 1.f, InAlpha, BlendExp);
		case EEasingFunc::ExpoIn:				return FMath::InterpExpoIn<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::ExpoOut:				return FMath::InterpExpoOut<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::ExpoInOut:			return FMath::InterpExpoInOut<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::CircularIn:			return FMath::InterpCircularIn<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::CircularOut:			return FMath::InterpCircularOut<float>(0.f, 1.f, InAlpha);
		case EEasingFunc::CircularInOut:		return FMath::InterpCircularInOut<float>(0.f, 1.f, InAlpha);
		default:								return InAlpha;
	}
}

float UKidRedFunctionLibrary::EaseClamped(const float A, const float B, const float Alpha, const TEnumAsByte<EEasingFunc::Type> EasingFunc, const float BlendExp, const int32 Steps)
{
	return FMath::Lerp(A, B, FMath::Clamp<float>(EaseAlpha(Alpha, EasingFunc, BlendExp, Steps), 0.0f, 1.0f));
}
