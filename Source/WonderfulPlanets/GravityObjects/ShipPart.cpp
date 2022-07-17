// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPart.h"

UShipPart::UShipPart()
{

}

void UShipPart::Init(int newPartID, FVector newPosition, FQuat newRotation, int newAttachment)
{
	partID = newPartID;
	partPosition = newPosition;
	partRotation = newRotation;
	attachedTo = newAttachment;
}
