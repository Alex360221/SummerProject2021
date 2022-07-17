// Fill out your copyright notice in the Description page of Project Settings.


#include "SolarSystemController.h"
#include <WonderfulPlanets/Planet/Mesh/PlanetController.h>
#include <WonderfulPlanets/GravityObjects/BaseShip.h>
#include <WonderfulPlanets/Player/BasePlayer.h>

// Sets default values
ASolarSystemController::ASolarSystemController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASolarSystemController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASolarSystemController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//	MovePlanets(FVector(1000, 0, 0));

}

void ASolarSystemController::Start()
{
	SpawnPlanets();
	SpawnShips();
}

void ASolarSystemController::SpawnPlanets()
{
	//spawn main planet
	APlanetController* newPC = GetWorld()->SpawnActor<APlanetController>(FVector(0, 0, 0), FRotator(0, 0, 0));
	if (newPC)
	{
		planets.Add(newPC);
		newPC->solarSystemController = this;
		newPC->playerRef = player;
		newPC->planetCentre = FVector(0, 0, /*-1000000*/0);
		player->nearestPlanet = newPC;
		newPC->hasBuildings = true;
		newPC->Start();		
	}
	//spawn Moon
	newPC = GetWorld()->SpawnActor<APlanetController>(FVector(-15000000, 0, -10000000), FRotator(0, 0, 0));
	if (newPC)
	{
		planets.Add(newPC);
		newPC->solarSystemController = this;
		newPC->playerRef = player;
		newPC->planetCentre = FVector(-20000000, 0, /*-1000000*/10000000);
		newPC->planetRadius = 1000000;
		newPC->hasAtomsphere = false;
		newPC->hasBuildings = false;
		newPC->Start();		
	}
}

void ASolarSystemController::SpawnShips()
{

	/*ABaseShip* newShip = GetWorld()->SpawnActor<ABaseShip>(FVector(-7060944.000000, 787018.562500, 7057716.000000)
		, FRotator(36.406162, -47.867847, -29.036823));
	if (newShip)
	{
		newShip->nearestPlanet = planets[0];
		newShip->solarSystemController = this;
		player->ship = newShip;
		ships.Add(newShip);
	}*/
}

void ASolarSystemController::AddShip(ABaseShip* newShip)
{
	if (planets.IsValidIndex(0)) { newShip->nearestPlanet = planets[0]; }
	player->ship = newShip;
	ships.Add(newShip);
}

void ASolarSystemController::RecentreSolarSystem(FVector shipPos)
{
	for (int i = 0; i < planets.Num(); i++)
	{
		planets[i]->MovePlanet(planets[i]->planetCentre - shipPos);
	}
}

void ASolarSystemController::MovePlanets(FVector movement)
{
	for (int i = 0; i < planets.Num(); i++)
	{
		planets[i]->MovePlanet(planets[i]->planetCentre + movement);
	}
}

