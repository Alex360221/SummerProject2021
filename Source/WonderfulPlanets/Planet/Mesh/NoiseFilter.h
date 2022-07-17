// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WONDERFULPLANETS_API NoiseFilter
{
public:
	NoiseFilter(class NoiseSettings* settings, class FastNoise* noise);
	float Evaluate(FVector point);
	bool useFirstLayerAsMask;
	class NoiseSettings* settings;
private:
	class FastNoise* noise;
};
