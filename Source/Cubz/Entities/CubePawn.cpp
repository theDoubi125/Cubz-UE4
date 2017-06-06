// Fill out your copyright notice in the Description page of Project Settings.

#include "CubePawn.h"
#include "Components/InputComponent.h"
#include "Movement/RollingMovementComponent.h"


ACubePawn::ACubePawn()
{
	PrimaryActorTick.bCanEverTick = true;
	MovementComponent = CreateDefaultSubobject<URollingMovementComponent>("Rolling Movement Component");
	MovementComponent->UpdatedComponent = RootComponent;
}

void ACubePawn::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent->RotationSpeed = RotationSpeed;
}

void ACubePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACubePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveX", this, &ACubePawn::SetInputX);
	PlayerInputComponent->BindAxis("MoveY", this, &ACubePawn::SetInputY);
}

void ACubePawn::SetInputX(float Intensity)
{
	InputDir.X = Intensity;
	if (!MovementComponent->IsMoving())
	{
		if (Intensity > 0)
			MovementComponent->StartStep(FVector(1, 0, 0), PI / 2, FVector(1, 0, -1));
		if (Intensity < 0)
			MovementComponent->StartStep(FVector(-1, 0, 0), PI / 2, FVector(-1, 0, -1));
	}
}

void ACubePawn::SetInputY(float Intensity)
{
	InputDir.Y = Intensity;
	if (!MovementComponent->IsMoving())
	{
		if (Intensity > 0)
			MovementComponent->StartStep(FVector(0, 1, 0), PI / 2, FVector(0, 1, -1));
		else if(Intensity < 0)
			MovementComponent->StartStep(FVector(0, -1, 0), PI / 2, FVector(0, -1, -1));
	}
}

UPawnMovementComponent* ACubePawn::GetMovementComponent() const
{
	return MovementComponent;
}