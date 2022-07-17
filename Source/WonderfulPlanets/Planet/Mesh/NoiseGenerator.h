// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WONDERFULPLANETS_API NoiseGenerator
{
public:
	NoiseGenerator();

	static FVector CalculatePointOnPlanet(FVector pointOnUnitSphere, float planetRadius, FVector planetCenter);
};
