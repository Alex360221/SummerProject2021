// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WONDERFULPLANETS_API NoiseSettings
{
public:
	NoiseSettings();
	float strength;
	int numLayers;
	float baseRoughness;
	float roughness;
	float persistence;
	FVector centre;
	float minVaule;
};
