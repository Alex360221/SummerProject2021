// Fill out your copyright notice in the Description page of Project Settings.


#include "ColourPicker.h"

ColourPicker::ColourPicker()
{
}

FColor ColourPicker::CalculateColour(FVector vertex, FVector planetCenter, float planetRaduis)
{
	//work out distance above surafce
		//work ot overal distance first

	float oDis = FVector::Dist(vertex, planetCenter);
	float aHeight = oDis - planetRaduis;
	if (aHeight < 0.f) { aHeight = 0.0f; }
	//oDis /= 100.f;
	//float maxHeight = 1000.f;	//this will be a set height for now
	////take the max int vaule for a colour which 244 and divide it by that max disatance then times by aHeight
	//float s = (255.f / maxHeight) * aHeight;

	//return FColor(s, 0, 0, 0);

	float localHighestPoint = 600000;
	float localLowestPoint = 0;
	if (localHighestPoint < 0) { localHighestPoint *= -1; }
	if (localLowestPoint < 0) { localLowestPoint *= -1; }
	float range = localHighestPoint + localLowestPoint;
	float percent = 255 / range;
	float aboveLowest = 0;
	if (aHeight > 0)
		{
		aboveLowest = aHeight + localLowestPoint;
		}
	if (aHeight < 0)
		{
		aboveLowest = localLowestPoint - (aHeight * -1);
		}
	int vertexColour = int(percent * aboveLowest);
	if (vertexColour < 0) { vertexColour = 0; }
	else if (vertexColour > 255) { vertexColour = 255; }
	return FColor(vertexColour, 0, 0, 0);
}