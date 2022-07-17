// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseShipPartEngine.h"

// Sets default values
ABaseShipPartEngine::ABaseShipPartEngine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	engineMaxThrust = 0.f;
	engineCurrentThrust = 0.f;
	engineThrustPoint = FVector(0, 0, 0);
	engineThrustDirection = FVector(0, 0, 0);
	engineMaxFuel = 0.f;
	engineCurrentFuel = 0.f;
}

//void ABaseShipPartEngine::BeginPlay()
//{
//}
//
//void ABaseShipPartEngine::Tick(float DeltaTime)
//{
//}

FVector ABaseShipPartEngine::GetThrust(float throttle)
{
	return FQuat(GetActorRotation()) * (engineThrustDirection * (engineMaxThrust / 100) * throttle);
}

FVector ABaseShipPartEngine::GetThrustPoint()
{
	return GetActorLocation() + (FQuat(GetActorRotation()) * engineThrustPoint) ;
}
