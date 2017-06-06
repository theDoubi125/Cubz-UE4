// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CubePawn.generated.h"

UCLASS()
class CUBZ_API ACubePawn : public APawn
{
	GENERATED_BODY()

public:
	ACubePawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;


	void SetInputX(float Intensity);
	void SetInputY(float Intensity);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float RotationSpeed;

protected:
	FVector2D InputDir;
	class URollingMovementComponent* MovementComponent;
};
