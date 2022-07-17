// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkyAtmosphereComponent.h"
#include "PlanetController.generated.h"

UCLASS()
class WONDERFULPLANETS_API APlanetController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetController();

	int vertexCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector rotationAxis;
	float planetRadius;
	FVector planetCentre;
	bool hasAtomsphere = true;
	bool hasBuildings = false;
	class ABasePlayer* playerRef;
	class ASolarSystemController* solarSystemController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkyAtmosphereComponent* skyAtomosphere;

	UPROPERTY()
		class ABaseShipBuildingCentre* shipBuilder;
private:
	TArray<class ABaseBuilding*> buildings;
	//(X=-7042230.000000,Y=774480.000000,Z=7074410.000000)
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float timer;

	void Start();
	//fucntions
	void SpawnChunks();
	void SpawnChunk(FVector chunkPos, FVector direction);

private:
	//stores the highest nodes  -6 nodes
	UPROPERTY()
		TArray<class UOctreeNodes*> parentNodes;
	//this will spawn the intial octree nodes
	void SpawnInitalNodes();
public:
	//this will spawn the planet mesh belonging to a octree node
	class APlanetMesh* SpawnChunk(class UOctreeNodes* node, FVector localUp, float meshStartX, float meshEndX, float meshStartY, float meshEndY);
	//this will add the palent mesh for destorying
	void AddPlanetMeshForDestorying(class APlanetMesh* planetMesh);
private:
	//stores all the planet meshes which need to be removed
	UPROPERTY()
		TArray<class APlanetMesh*> planetMeshesToDestory;
	//this will remove the planet meshes in this array
	void DestoryPlanetMeshes();
public:
	//will move the planet to this location, importantly moving all the meshes as well
	void MovePlanet(FVector newLocation);
};
