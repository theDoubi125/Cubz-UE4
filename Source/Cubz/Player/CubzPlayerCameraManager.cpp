// Fill out your copyright notice in the Description page of Project Settings.

#include "CubzPlayerCameraManager.h"
#include "Engine/World.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"

void ACubzPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	APawn* Pawn = GetOwningPlayerController()->GetPawn();
	if (IsValid(Pawn))
	{
		FVector Direction = FRotator(Pitch, Yaw, 0).RotateVector(FVector(DistanceToTarget, 0, 0));
		CameraCache.POV.Location = Pawn->GetActorLocation() - Direction;
		CameraCache.POV.Rotation = FRotator(Pitch, Yaw, 0);
		CameraCache.TimeStamp = GetWorld()->TimeSeconds;
	}
	
}

void ACubzPlayerCameraManager::RotateCamera(float DeltaYaw, float DeltaPitch)
{
	Yaw += DeltaYaw;
	Pitch += DeltaPitch;
}