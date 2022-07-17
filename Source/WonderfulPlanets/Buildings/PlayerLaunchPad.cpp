// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLaunchPad.h"
#include <WonderfulPlanets/Saving/ShipSaveData.h>
#include "Kismet/GameplayStatics.h"
#include <WonderfulPlanets/GravityObjects/ShipPart.h>
#include <WonderfulPlanets/GravityObjects/BaseShip.h>
#include <WonderfulPlanets/Galaxy/SolarSystemController.h>



APlayerLaunchPad::APlayerLaunchPad()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/SpaceCentre/Launchpad.Launchpad'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);

}

void APlayerLaunchPad::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerLaunchPad::Start()
{
	SpawnShip();
}

void APlayerLaunchPad::SpawnShip()
{
	//load ship 
	TArray<UShipPart*> shipPartsInfo;
	int cockpitIndex = 0;
	if (UShipSaveData* loadedShip = Cast<UShipSaveData>(UGameplayStatics::LoadGameFromSlot(FString("ship1"), 0)))
	{
		//loading the ship was succesful so set data
		GLog->Log("SUCCESFULLY LOADED SHIP");

		for (int i = 0; i < loadedShip->partID.Num();i++)
		{
			UShipPart* newPart = NewObject<UShipPart>();
			newPart->Init(loadedShip->partID[i], loadedShip->partPosition[i],
				loadedShip->partRotation[i], loadedShip->attachedTo[i]);
			shipPartsInfo.Add(newPart);
			//sets the mastercockpit index
			if (loadedShip->partID[i] == 1) { cockpitIndex = i; }
		}
	}
	else
	{
		//use defualt ship for now
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[0]->Init(1, FVector(0, 0, 700), FQuat(FRotator(0, 0, 0)), 0);
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[1]->Init(0, FVector(0, 0, 0), FQuat(FRotator(0, 0, 0)), 0);
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[2]->Init(0, FVector(-100, 0, 0), FQuat(FRotator(0, 0, 0)), 1);
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[3]->Init(0, FVector(100, 0, 0), FQuat(FRotator(0, 0, 0)), 1);
	}

	//FVector(-7060944.000000, 787018.562500, 7057716.000000)
	FVector t = GetActorQuat() * (shipPartsInfo[cockpitIndex]->partPosition + FVector(0,0,2000));
	UE_LOG(LogTemp, Warning, TEXT("part location is: %s"), *shipPartsInfo[cockpitIndex]->partPosition.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Ship part spawn location is: %s"), *t.ToString());
	ABaseShip* newShip = GetWorld()->SpawnActor<ABaseShip>(GetActorLocation() + t
		, FRotator(36.406162, -47.867847, -29.036823));

	if (newShip && solarSystemController)
	{
		//newShip->nearestPlanet = planets[0];
		newShip->solarSystemController = solarSystemController;
		//player->ship = newShip;
		//ships.Add(newShip);
		solarSystemController->AddShip(newShip);
	}
}
