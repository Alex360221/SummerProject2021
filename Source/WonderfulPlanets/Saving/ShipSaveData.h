// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShipSaveData.generated.h"

/**
 * 
 */
UCLASS()
class WONDERFULPLANETS_API UShipSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
		TArray<int> partID;
	UPROPERTY(VisibleAnywhere)
		TArray<FVector> partPosition;
	UPROPERTY(VisibleAnywhere)
		TArray<FQuat> partRotation;
	UPROPERTY(VisibleAnywhere)
		TArray<int> attachedTo;

	UShipSaveData();

};
