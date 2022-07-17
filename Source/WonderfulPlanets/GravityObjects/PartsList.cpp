// Fill out your copyright notice in the Description page of Project Settings.


#include "PartsList.h"
#include "BaseShipPart.h"
#include "Engines/EngineTest.h"
#include "Cockpits/CockpitTest.h"

PartsList::PartsList()
{
}

TSubclassOf<class ABaseShipPart> PartsList::GetShipPart(int partID)
{
	switch (partID)
	{
	default:
		break;
	case 0:
		return AEngineTest::StaticClass();
		break;
	case 1:
		return ACockpitTest::StaticClass();
	}
	return TSubclassOf<class ABaseShipPart>();
}

float PartsList::GetShipPartWeight(int partID)
{

	
	return 0.0f;
}

