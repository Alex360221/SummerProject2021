// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityCalculator.h"
#include "Mesh/PlanetController.h"

GravityCalculator::GravityCalculator()
{
}

FVector GravityCalculator::CalculateGravityForObject(float massOfObject, FVector positionOfObject, TArray<class APlanetController*> planets)
{
	FVector gravity = FVector(0, 0, 0);
	//looping through planets as there might be more than one gravitional body effecting this object
	for (int i = 0; i < planets.Num();i++)
	{
		// work out gravitional pull
		float gravitationalConstant = 9.87;
		float massOfPlanet = 100;
		float distance = FVector::Dist(positionOfObject, planets[i]->planetCentre);
		float force = gravitationalConstant * ((massOfObject * massOfPlanet) / distance * distance);
		//direction between object and planet
		FVector direction = planets[i]->planetCentre - positionOfObject;
		direction.Normalize();
		gravity += direction * force;
	}
	return gravity;
}
FVector GravityCalculator::CalculateGravityForObject(float massOfObject, FVector positionOfObject, FVector planets)
{
	FVector gravity = FVector(0, 0, 0);
	//looping through planets as there might be more than one gravitional body effecting this object
	// work out gravitional pull
	float gravitationalConstant = 9.87;
	float massOfPlanet = 50000;
	float distance = FVector::Dist(positionOfObject, planets);
	float force = gravitationalConstant * ((massOfObject * massOfPlanet) / distance * distance);
	//direction between object and planet
	FVector direction = planets - positionOfObject;
	direction.Normalize();
	gravity += direction * force;

	return gravity;
}
