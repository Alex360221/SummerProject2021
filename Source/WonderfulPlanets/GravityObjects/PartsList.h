// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"

/**
 * 
 */
class WONDERFULPLANETS_API PartsList
{
public:
	PartsList();
	//returns the spefic class belonging to this partID
	static TSubclassOf<class ABaseShipPart> GetShipPart(int partID);
	//returns the weight of this partID
	static float GetShipPartWeight(int partID);
	
};
