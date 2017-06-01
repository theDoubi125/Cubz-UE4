// Fill out your copyright notice in the Description page of Project Settings.

#include "Cube.h"


ACube::ACube()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACube::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACube::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

