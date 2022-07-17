// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseGenerator.h"
#include <WonderfulPlanets/Lib/Noise/FastNoise.h>
#include "NoiseFilter.h"
#include "NoiseSettings.h"

NoiseGenerator::NoiseGenerator()
{

}

FVector NoiseGenerator::CalculatePointOnPlanet(FVector pointOnUnitSphere, float planetRadius, FVector planetCenter)
{
	pointOnUnitSphere.Normalize();
	TArray<class NoiseFilter*> noiseFilters;
	FastNoise* fn = new FastNoise();
	fn->SetNoiseType(FastNoise::Perlin);
	fn->SetSeed(7676);
	for (int i = 0; i < 2; i++)
	{
		//settings = new NoiseSettings();
		NoiseSettings* ns = new NoiseSettings();
		ns->centre = planetCenter;
		NoiseFilter* nf = new NoiseFilter(ns, fn);
		noiseFilters.Add(nf);
	}
	//noiseFilters[0]->settings->strength = 0.3;//0,3
	//noiseFilters[0]->settings->baseRoughness = 3.3;
	//noiseFilters[0]->settings->roughness = 1.2;
	//noiseFilters[0]->settings->persistence = 0.6;
	//noiseFilters[0]->settings->minVaule = 1.2;

	noiseFilters[0]->settings->strength = 0.08;//0,3
	noiseFilters[0]->settings->baseRoughness = 4.5;
	noiseFilters[0]->settings->roughness = 1.1;
	noiseFilters[0]->settings->persistence = 0.65;
	noiseFilters[0]->settings->minVaule = 1.27;

	noiseFilters[1]->settings->strength = 2.0;
	noiseFilters[1]->settings->baseRoughness = 2.03;
	noiseFilters[1]->settings->roughness = 3.0156;
	noiseFilters[1]->settings->persistence = 0.54;
	noiseFilters[1]->settings->minVaule = 1.1;
	noiseFilters[1]->useFirstLayerAsMask = true;

	float firstLayerVaule = 0;
	float elevation = 0;
	if (noiseFilters.Num() > 0)
	{
		firstLayerVaule = noiseFilters[0]->Evaluate(pointOnUnitSphere);
		elevation = firstLayerVaule;
	}
	for (int i = 1; i < noiseFilters.Num();i++)
	{
		float mask = (noiseFilters[i]->useFirstLayerAsMask) ? firstLayerVaule : 1;
		elevation += noiseFilters[i]->Evaluate(pointOnUnitSphere) * mask;
	}
	elevation = planetRadius * (1 + elevation);
	
	FVector p = pointOnUnitSphere * planetRadius;
	//(X=-7059880.000000,Y=786690.000000,Z=7065010.000000)
	FVector spawnPoint = FVector(-7059880, 786690, 7065010);
	float spawnFlatArea = 100000;
	if (p.X > (spawnPoint.X - spawnFlatArea) && p.X < (spawnPoint.X + spawnFlatArea)
		&& p.Y >(spawnPoint.Y - spawnFlatArea) && p.Y < (spawnPoint.Y + spawnFlatArea)
		&& p.Z >(spawnPoint.Z - spawnFlatArea) && p.Z < (spawnPoint.Z + spawnFlatArea))
	{
		elevation = planetRadius + 12000.f;
	}
	else
	{
		//elevation = elevation;
	}

	//elevationMinMax->AddVaule(elevation);
	//return pointOnUnitSphere;// *planetRadius;

	//delete pointers
	delete fn;
	fn = NULL;
	delete noiseFilters[0];
	noiseFilters[0] = NULL;
	delete noiseFilters[1];
	noiseFilters[1] = NULL;

	return pointOnUnitSphere * elevation;
}

