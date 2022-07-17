// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseFilter.h"
#include "NoiseSettings.h"
#include <WonderfulPlanets/Lib/Noise/FastNoise.h>


NoiseFilter::NoiseFilter(class NoiseSettings* settings, class FastNoise* noise)
{
	this->settings = settings;
	this->noise = noise;
	useFirstLayerAsMask = false;
}

float NoiseFilter::Evaluate(FVector point)
{
	float noiseValue = 0;
	float frequency = settings->baseRoughness;
	float ampltiude = 1;

	for (int i = 0; i < settings->numLayers;i++)
	{
		//point = point * settings->roughness + settings->centre;
		//noiseValue = noise->GetNoise(point.X, point.Y, point.Z);
		point = ((point * frequency)/**0.5*/) + settings->centre;
		float v = noise->GetNoise(point.X, point.Y, point.Z);
		noiseValue += (v + 1) * .5f * ampltiude;
		frequency *= settings->roughness;		//when roughness is greater than 1 frequency will increase
		ampltiude *= settings->persistence;		//when persistence is less than 1 amplitude will deacrease
	}
	//limits the noise change this to stop it having a flat ocean
	noiseValue = FMath::Max<float>(0, noiseValue - settings->minVaule);
	return noiseValue * settings->strength;
}
