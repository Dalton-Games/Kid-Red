// Copyright 2021 Kid Red. All Rights Reserved.

#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "KidRedAnimInstance.generated.h"

UENUM(BlueprintType)
enum class ECharacterAnimState : uint8
{
	Any,
	Unarmed,
	Armed
};

USTRUCT(BlueprintType)
struct FCharacterAnimData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<class UAnimMontage> AnimMontage = nullptr;
};

USTRUCT(BlueprintType)
struct FAnimationTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<ECharacterAnimState, FCharacterAnimData> AnimationData;
};

USTRUCT(BlueprintType)
struct FKidRedAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

	FKidRedAnimInstanceProxy();
	FKidRedAnimInstanceProxy(UAnimInstance* InAnimInstance);

	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	virtual void PostUpdate(UAnimInstance* InAnimInstance) const override;
	
	UPROPERTY()
	UWorld* World = nullptr;
};

/**
 * Base class anim instance for all Kid Red characters
 */
UCLASS(Transient, Blueprintable, BlueprintType) 
class KIDREDCORE_API UKidRedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	friend struct FKidRedAnimInstanceProxy;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float DeltaTime = 0.0f;
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Character Data", meta = (AllowPrivateAccess = true))
	AKidRedCharacter* KidRedCharacter = nullptr;

	// Expose to Blueprint but only to graphs internal to this class
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FKidRedAnimInstanceProxy Proxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return new FKidRedAnimInstanceProxy(this);
	}
};
