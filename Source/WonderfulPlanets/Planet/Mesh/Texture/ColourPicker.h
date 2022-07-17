// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class WONDERFULPLANETS_API ColourPicker
{
public:
	ColourPicker();
	//this will work out the colour for this vertex
	static FColor CalculateColour(FVector vertex, FVector planetCenter, float planetRaduis);
};
