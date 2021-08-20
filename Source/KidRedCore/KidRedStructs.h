#pragma once

#include "KidRedStructs.generated.h"

USTRUCT(BlueprintType)
struct FCharacterProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Profile")
	FString CharacterName = "Default";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Profile")
	FText CharacterNameUI = FText::FromString("Default");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Profile")
	int32 Age = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Profile")
	float Height = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Profile")
	ECharacterGender Gender = ECharacterGender::Male;
};
