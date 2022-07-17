// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetController.h"
#include "PlanetMesh.h"
#include "OctreeNodes.h"
#include <WonderfulPlanets/Player/BasePlayer.h>
#include <WonderfulPlanets/ShipBuilding/BaseShipBuildingCentre.h>

//other test buildings
#include <WonderfulPlanets/Buildings/BaseBuilding.h>
#include <WonderfulPlanets/Buildings/PlayerLaunchPad.h>
#include <WonderfulPlanets/Buildings/BuildingTest1.h>
#include <WonderfulPlanets/Buildings/BuildingTest2.h>
#include <WonderfulPlanets/Buildings/BuildingTest3.h>

// Sets default values
APlanetController::APlanetController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	vertexCount = 50;
	rotationAxis = FVector(0, -1, 1);
	rotationSpeed = 0.0001f;
	planetRadius = 10000000;
	planetCentre = FVector(0, 0, 0);

	//skyAtomosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("Sky Atomosphere"));
	//SetRootComponent(skyAtomosphere);

	////intitalise atomsphere
	//skyAtomosphere->BottomRadius = 100;
	//skyAtomosphere->AtmosphereHeight = 20;
	//skyAtomosphere->TransformMode = ESkyAtmosphereTransformMode::PlanetCenterAtComponentTransform;

}

// Called when the game starts or when spawned
void APlanetController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlanetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timer += DeltaTime;
	if (timer >= 0.4f)
	{
		timer = 0.f;
		if (!playerRef) { return; }
		FVector playerPos = playerRef->GetActorLocation();
		for (int i = 0; i < parentNodes.Num();i++)
		{
			//GLog->Log("update .. " + FString::FromInt(i));
			parentNodes[i]->Update(playerPos);
		}

		//remove plaent meshes
		DestoryPlanetMeshes();
	}
}

void APlanetController::Start()
{
	//set up atomsphere
	if (hasAtomsphere)
	{
		
		skyAtomosphere = NewObject<USkyAtmosphereComponent>(this,TEXT("sky atompshere")); /*<USkyAtmosphereComponent>(TEXT("Sky Atomosphere"));*/
		if (skyAtomosphere)
		{
			//intitalise atomsphere
			skyAtomosphere->BottomRadius = planetRadius / 100000;
			skyAtomosphere->AtmosphereHeight = 20;
			skyAtomosphere->TransformMode = ESkyAtmosphereTransformMode::PlanetCenterAtComponentTransform;
			skyAtomosphere->RegisterComponent();
		//	skyAtomosphere->AttachTo(GetRootComponent());
			SetRootComponent(skyAtomosphere);
		}
	}


	SpawnInitalNodes();
	if (hasBuildings)
	{
		//spawn buildings
		shipBuilder = GetWorld()->SpawnActor<ABaseShipBuildingCentre>(FVector(-7042010.000000, 774480.000000, 7074960.000000), FRotator(45.017265, -4.545594, 0.615704));
		FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, true);
		shipBuilder->AttachToActor(this, attachRules);

		APlayerLaunchPad* launchPad = GetWorld()->SpawnActor<APlayerLaunchPad>(FVector(-7057830.000000, 786960.000000, 7057620.000000),
			FRotator(44.782379, 4.981074, 7.053236));
		if (launchPad)
		{
			launchPad->solarSystemController = solarSystemController;
			launchPad->Start();
		}
		buildings.Add(launchPad);
		buildings[0]->AttachToActor(this, attachRules);
		

		buildings.Add(GetWorld()->SpawnActor<ABuildingTest2>(FVector(-7010470.000000, 770070.000000, 7106440.000000),
			FRotator(39.955547, 21.941771, 16.971781)));
		buildings[1]->AttachToActor(this, attachRules);

		buildings.Add(GetWorld()->SpawnActor<ABuildingTest3>(FVector(-7049480.000000, 737250.000000, 7069870.000000),
			FRotator(44.724686, 5.598442, 7.929672)));
		buildings[2]->AttachToActor(this, attachRules);

		buildings.Add(GetWorld()->SpawnActor<ABuildingTest3>(FVector(-7049490.000000, 756740.000000, 7069100.000000),
			FRotator(44.724686, 5.598442, 7.929672)));
		buildings[3]->AttachToActor(this, attachRules);
	}	
	//SpawnChunks();
}

void APlanetController::SpawnChunks()
{
	FVector actorPos = GetActorLocation();
//	float halfChunk = chunkSize / 2;
	TArray<FVector> directions = { FVector::UpVector, FVector::DownVector, FVector::LeftVector,
		FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };
	for (int i = 0; i < 6;i++)
	{
		SpawnChunk(actorPos, directions[i]);
	}
	//	colourGenerator->UpdateElevation(shapeGenerator->elevationMinMax);
	//	colourGenerator->UpdateColour();
}

void APlanetController::SpawnChunk(FVector chunkPos, FVector direction)
{
	//FVector newChunkPos = FVector(chunkPos.X, chunkPos.Y, 0);	
	FRotator rotation = FRotator::ZeroRotator;
	APlanetMesh* mesh = GetWorld()->SpawnActor<APlanetMesh>(chunkPos, rotation);
	//	chunks.Add(chunkPos, mesh);
	//planetMeshes.Add(mesh);
	if (mesh != nullptr)
	{
		//mesh->mapRef = this;
	//	mesh->shapeGenerator = shapeGenerator;
		mesh->vertexCount = vertexCount;
		//		mesh->vertexDistance = vertexDistance;
		//mesh->playerRef = this->playerRef;
		mesh->localUp = direction;
		//mesh->mapController = this;
		mesh->Start();
	}
}

void APlanetController::SpawnInitalNodes()
{
	TArray<FVector> directions = { FVector::UpVector, FVector::DownVector, FVector::LeftVector,
		FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };
	for (int i = 0; i < 6; i++)
	{
		GLog->Log("Spawn octree Node ");
		int n = parentNodes.Add(NewObject<UOctreeNodes>());
		parentNodes[n]->Init(this, nullptr, 0, 0.f, 1.f, 0.f, 1.f, directions[i], vertexCount, 0);
		//parentNodes.Add(new OctreeNode(this, nullptr, 0, 0.f, 1.f, 0.f, 1.f, directions[i], vertexCount, 0));
	}
}

APlanetMesh* APlanetController::SpawnChunk(class UOctreeNodes* node, FVector localUp, float meshStartX, float meshEndX, float meshStartY, float meshEndY)
{
	FRotator rotation = FRotator::ZeroRotator;
	APlanetMesh* mesh = GetWorld()->SpawnActor<APlanetMesh>(planetCentre, rotation);
	if (mesh)
	{
		mesh->node = node;
		mesh->localUp = localUp;
		mesh->planetRadius = planetRadius;
		mesh->vertexCount = vertexCount;
		mesh->meshStartX = meshStartX;
		mesh->meshStartY = meshStartY;
		mesh->meshEndX = meshEndX;
		mesh->meshEndY = meshEndY;

		mesh->Start();
	}
	return mesh;
}

void APlanetController::AddPlanetMeshForDestorying(APlanetMesh* planetMesh)
{
	if (planetMesh)
	{
		//make them invisble to save gpu power
		planetMesh->SetActorHiddenInGame(false);
		planetMeshesToDestory.Add(planetMesh);
		GLog->Log("Meshes for destorying : " + FString::FromInt(planetMeshesToDestory.Num()));
	}
}

void APlanetController::DestoryPlanetMeshes()
{
	int maxCount = 10;
	if (planetMeshesToDestory.Num() < maxCount) { maxCount = planetMeshesToDestory.Num(); }
	//uses a limit of max count so not all the chunks are destroyed in one tick
	for (int i = 0; i < maxCount; i++)
	{
		if (planetMeshesToDestory[0]) { planetMeshesToDestory[0]->DestroyMesh(); }
		planetMeshesToDestory.RemoveAt(0);
	}
}

void APlanetController::MovePlanet(FVector newLocation)
{
	SetActorLocation(newLocation);
	planetCentre = newLocation;
	for (int i = 0; i < parentNodes.Num();i++)
	{
		parentNodes[i]->MoveMeshes(newLocation);
	}
}
