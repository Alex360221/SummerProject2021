// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShipPart.h"
#include "BaseShipPartEngine.generated.h"

/**
 * 
 */
UCLASS()
class WONDERFULPLANETS_API ABaseShipPartEngine : public ABaseShipPart
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseShipPartEngine();

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

	//Engine variables
	float engineMaxThrust;
	float engineCurrentThrust;
	float engineMaxFuel;
	float engineCurrentFuel;
	FVector engineThrustPoint;
	FVector engineThrustDirection;

	FVector GetThrust(float throttle);
	FVector GetThrustPoint();
	
};
