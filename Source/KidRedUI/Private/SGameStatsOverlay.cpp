// Copyright 2021 Ali El Saleh. All Rights Reserved.

#include "SGameStatsOverlay.h"

#include "HelperFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "GameStatsOverlay"

void SGameStatsOverlay::Construct(const FArguments& InArgs)
{
	const FText FPSCountLabel = LOCTEXT("FPSCount", "FPS");
	
	FPSCounterValue = SAssignNew(FPSCounterValue, STextBlock);
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			
			// Main containter box
			+ SHorizontalBox::Slot()
			[
				// FPS Counter Box
				SNew(SHorizontalBox)
				
				// FPS Counter Value
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					FPSCounterValue.ToSharedRef()
				]
				
				// FPS Counter Label
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Margin(FMargin(2.0f, 0.0f, 0.0f, 0.0f))
					.Text(FPSCountLabel)
				]
			]

			// Build stats container box
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			[
				SNew(SVerticalBox)
				
				// Main vertical box container
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SHorizontalBox)

					// Build name
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(STextBlock)
						.Text(FText::FromString(UHelperFunctionLibrary::GetProjectName()))
					]

					// Build config
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(STextBlock)
						.Margin(FMargin(2.0f, 0.0f, 0.0f, 0.0f))
						.Text(FText::FromString(UHelperFunctionLibrary::GetBuildConfigurationAsString()))
					]
				]

				// Build version
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				[
					SNew(STextBlock)
					.Text(FText::FromString(UHelperFunctionLibrary::GetProjectVersion()))
				]
			]
		]
	];
}

void SGameStatsOverlay::Tick(const FGeometry& MovieSceneBlends, const double InCurrentTime, const float InDeltaTime)
{
	if (FPSCounterValue.Get())
	{
		FNumberFormattingOptions FPSTextFormatting;
		FPSTextFormatting.RoundingMode = HalfToEven;
		FPSTextFormatting.MinimumIntegralDigits = 1;
		FPSTextFormatting.MaximumIntegralDigits = 5;
		FPSTextFormatting.MinimumFractionalDigits = 0;
		FPSTextFormatting.MaximumFractionalDigits = 0;
		
		FPSCounterValue->SetText(FText::AsNumber(1.0f/InDeltaTime, &FPSTextFormatting));
	}
}

#undef LOCTEXT_NAMESPACE
