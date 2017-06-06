// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubzPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CUBZ_API ACubzPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
	
	template<int8 dirX, int8 dirY>
	void RotateCamera(float Intensity);
	
};
