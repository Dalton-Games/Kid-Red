// Copyright 2021 Kid Red. All Rights Reserved.

#include "KidRedAnimInstance.h"

FKidRedAnimInstanceProxy::FKidRedAnimInstanceProxy()
{
}

FKidRedAnimInstanceProxy::FKidRedAnimInstanceProxy(UAnimInstance* InAnimInstance)
	: Super(InAnimInstance)
{
}

void FKidRedAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, const float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);
}

void FKidRedAnimInstanceProxy::Update(const float DeltaSeconds)
{

}

void FKidRedAnimInstanceProxy::PostUpdate(UAnimInstance* InAnimInstance) const
{
	Super::PostUpdate(InAnimInstance);
	
	// Copy data back to external objects that need them
}

void UKidRedAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	KidRedCharacter = Cast<AKidRedCharacter>(TryGetPawnOwner());
}

void UKidRedAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	DeltaTime = DeltaSeconds;
}
