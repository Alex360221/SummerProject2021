// Fill out your copyright notice in the Description page of Project Settings.


#include "DragCalculator.h"

DragCalculator::DragCalculator()
{
}

FVector DragCalculator::CalculateDragForObject(float dragCoefficient, float airDensity, FVector velocity, float area)
{
	//no air so no no drag
	if (airDensity <= 0.f) { return FVector(0, 0, 0); }
	FVector velocitySquaredp = velocity * velocity;
	FVector drag = FVector(0, 0, 0);
	if (velocitySquaredp.X < 0) { velocitySquaredp.X *= -1; }
	if (velocitySquaredp.Y < 0) { velocitySquaredp.Y *= -1; }
	if (velocitySquaredp.Z < 0) { velocitySquaredp.Z *= -1; }

	drag.X = (dragCoefficient * area * 0.5 * airDensity * velocitySquaredp.X);
	if (velocity.X > 0.0f) { drag.X *= -1; }

	drag.Y = (dragCoefficient * area * 0.5 * airDensity * velocitySquaredp.Y);
	if (velocity.Y > 0.0f) { drag.Y *= -1; }

	drag.Z = (dragCoefficient * area * 0.5 * airDensity * velocitySquaredp.Z);
	if (velocity.Z > 0.0f) { drag.Z *= -1; }

	//float x = (dragCoefficient * area * 0.5 * airDensity * velocitySquared.X) /** -1*/;
	//float y = (dragCoefficient * area * 0.5 * airDensity * velocitySquared.Y) /** -1*/;
	//float z = (dragCoefficient * area * 0.5 * airDensity * velocitySquared.Z)/* * -1*/;
	//adjust in axis to equal oppersit of velocity
	return drag;//(dragCoefficient * area * 0.5 * airDensity * velocitySquared)/**-1*/;
}
