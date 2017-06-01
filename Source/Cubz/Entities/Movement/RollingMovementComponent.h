// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "RollingMovementComponent.generated.h"

UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = (Velocity))
class CUBZ_API URollingMovementComponent : public UMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void StartStep(FVector Direction, float Angle, FVector Pivot);

	UFUNCTION(BlueprintCallable, Category="Movement")
	bool IsMoving() const;

	void SetRotationSpeed(float InRotationSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Rotation")
	float RotationSpeed;

private:
	FQuat TargetRotation;
	FVector TargetPosition;
	FVector Pivot;
	FRotator RotationRate;
	float AnimationTime;
	float AnimationDuration;
};
