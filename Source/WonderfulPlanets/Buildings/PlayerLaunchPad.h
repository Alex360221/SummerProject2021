// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "PlayerLaunchPad.generated.h"

/**
 * 
 */
UCLASS()
class WONDERFULPLANETS_API APlayerLaunchPad : public ABaseBuilding
{
	GENERATED_BODY()
public:
	APlayerLaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//called after variables have been init
	void Start();
	//spawn ship on launch pad for player
	void SpawnShip();

	class ASolarSystemController* solarSystemController;
	
};
