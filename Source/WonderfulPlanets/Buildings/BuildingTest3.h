// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "BuildingTest3.generated.h"

/**
 * 
 */
UCLASS()
class WONDERFULPLANETS_API ABuildingTest3 : public ABaseBuilding
{
	GENERATED_BODY()

public:
	ABuildingTest3();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mesh1;
	
};
