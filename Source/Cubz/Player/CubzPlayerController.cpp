// Fill out your copyright notice in the Description page of Project Settings.

#include "CubzPlayerController.h"
#include "CubzPlayerCameraManager.h"

void ACubzPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("RotateCameraX", this, &ACubzPlayerController::RotateCamera<1, 0>);
	InputComponent->BindAxis("RotateCameraY", this, &ACubzPlayerController::RotateCamera<0, 1>);
}

template<int8 dirX, int8 dirY>
void ACubzPlayerController::RotateCamera(float Intensity)
{
	ACubzPlayerCameraManager* CameraManager = Cast<ACubzPlayerCameraManager>(PlayerCameraManager);
	if (IsValid(CameraManager))
	{
		CameraManager->RotateCamera(Intensity * dirX, - Intensity * dirY);
	}
}