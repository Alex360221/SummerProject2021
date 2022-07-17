// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShipPart.generated.h"

/**
 * 
 */
UCLASS()
class WONDERFULPLANETS_API UShipPart : public UObject
{
	GENERATED_BODY()

public:
	UShipPart();

	void Init(int newPartID, FVector newPosition, FQuat newRotation, int newAttachment);

	int partID;
	FVector partPosition;
	FQuat partRotation;
	int attachedTo;

};
