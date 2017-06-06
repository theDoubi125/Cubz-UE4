// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CubzPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class CUBZ_API ACubzPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	virtual void UpdateCamera(float DeltaTime) override;
	
	void RotateCamera(float DeltaYaw, float DeltaPitch);

	UPROPERTY(EditAnywhere, Category = "Camera")
	float DistanceToTarget;
private:
	float Yaw, Pitch;
	class AActor* ControlledCamera;
	
};
