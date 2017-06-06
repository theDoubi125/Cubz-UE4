// Fill out your copyright notice in the Description page of Project Settings.

#include "RollingMovementComponent.h"


URollingMovementComponent::URollingMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AnimationTime = -1;
	
}

bool URollingMovementComponent::IsMoving() const
{
	return AnimationTime >= 0;
}

void URollingMovementComponent::SetRotationSpeed(float InRotationSpeed)
{
	RotationSpeed = InRotationSpeed;
}

void URollingMovementComponent::StartStep(FVector Direction, float Angle, FVector InPivot)
{
	FVector RotationAxis = FVector::CrossProduct(Direction, InPivot);
	Pivot = InPivot;
	AnimationDuration = Angle / RotationSpeed;
	RotationRate = FRotator(FQuat(RotationAxis, RotationSpeed));
	AnimationTime = 0;
	TargetRotation = FQuat(RotationAxis, Angle) * UpdatedComponent->GetComponentQuat();
	TargetPosition = UpdatedComponent->GetComponentLocation();// +InPivot - FQuat(RotationAxis, Angle).RotateVector(InPivot);
}

void URollingMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(UpdatedComponent))
	{
		return;
	}

	if (IsMoving())
	{
		AnimationTime += DeltaTime;
		if (AnimationTime >= AnimationDuration)
		{
			DeltaTime -= AnimationTime - AnimationDuration;
			AnimationTime = -1;
		}
		// Compute new rotation
		const FQuat OldRotation = UpdatedComponent->GetComponentQuat();
		FVector Axis;
		float Angle;
		RotationRate.Quaternion().ToAxisAndAngle(Axis, Angle);
		const FQuat DeltaRotation = FQuat(Axis, RotationSpeed * DeltaTime);
		const FQuat NewRotation = DeltaRotation * OldRotation;

		// Compute new location
		FVector DeltaLocation = FVector::ZeroVector;
		if (!Pivot.IsZero())
		{
			const FVector OldPivot = OldRotation.RotateVector(Pivot);
			const FVector NewPivot = NewRotation.RotateVector(Pivot);
			DeltaLocation = (OldPivot - NewPivot);
		}
		MoveUpdatedComponent(DeltaLocation, NewRotation, false);
	}
}