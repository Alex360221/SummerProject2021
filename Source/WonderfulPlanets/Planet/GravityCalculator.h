// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WONDERFULPLANETS_API GravityCalculator
{
public:
	GravityCalculator();

	//Works out the force of gravity on this object
	static FVector CalculateGravityForObject(float massOfObject, FVector positionOfObject, TArray<class APlanetController*> planets);
	static FVector CalculateGravityForObject(float massOfObject, FVector positionOfObject, FVector planets);
};
