// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WONDERFULPLANETS_API DragCalculator
{
public:
	DragCalculator();

	//Calculates the drag force for theses settings
	static FVector CalculateDragForObject(float dragCoefficient, float airDensity, FVector velocity, float area);
};
