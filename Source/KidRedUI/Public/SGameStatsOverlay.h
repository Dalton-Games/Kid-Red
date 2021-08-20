// Copyright 2021 Ali El Saleh. All Rights Reserved.

#pragma once

#include "SlateBasics.h"

/**
 * 
 */
class SGameStatsOverlay : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameStatsOverlay) {}
	SLATE_ARGUMENT(TWeakObjectPtr<class AKidRedHUD>, OwningHUD)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	TWeakObjectPtr<class AKidRedHUD> OwningHUD;
	TSharedPtr<STextBlock> FPSCounterValue;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
