// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SolarSystemController.generated.h"

UCLASS()
class WONDERFULPLANETS_API ASolarSystemController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASolarSystemController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//called once variables have been set after spawning
	void Start();

	class ABasePlayer* player;
private:
	TArray<class APlanetController*> planets;
	TArray<class ABaseShip*> ships;

	//this will spawn all the planets in this galaxy
	void SpawnPlanets();

	//this will spawn all the ships in this solar system
	void SpawnShips();
public:
	//this will finish setting up a new ship called from luanch pad
	void AddShip(class ABaseShip* newShip);

public:
	//this will recentre solar sytem so the ship is at (0,0,0)
	void RecentreSolarSystem(FVector shipPos);
	//this will move all the planets by this vector
	void MovePlanets(FVector movement);
};
