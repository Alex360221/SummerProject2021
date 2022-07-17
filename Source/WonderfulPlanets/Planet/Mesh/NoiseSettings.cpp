// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseSettings.h"

NoiseSettings::NoiseSettings()
{
	strength = 0.6;
	numLayers = 5;
	baseRoughness = 1.2;
	roughness = 3;
	persistence = 0.54f;
	centre = FVector(0, 0, 0);
	minVaule = 1.1;
}
