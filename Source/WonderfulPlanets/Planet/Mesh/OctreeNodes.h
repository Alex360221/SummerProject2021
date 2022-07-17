// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OctreeNodes.generated.h"

/**
 * 
 */
UCLASS()
class WONDERFULPLANETS_API UOctreeNodes : public UObject
{
	GENERATED_BODY()
	
public:
	UOctreeNodes();

	void Init(class APlanetController* newPlanetController, UOctreeNodes* newParentNode, int newOctreeLevel, float newMeshStartX, float newMeshEndX,
		float newMeshStartY, float newMeshEndY, FVector newLocalUp, int newVertexCount, int newOctreeIndex);

	//this will be called to update this octree and all its child
	void Update(FVector playerPos);

	int octreeIndex, octreeLevel;
	FVector meshCentre, localUp;

	class APlanetController* planetController;
	class APlanetMesh* planetMesh;
	UPROPERTY()
		UOctreeNodes* parentNode;
	//distances are stored in an array so there can be more controll over when the LOD loads
	TArray<float> targetDistance;

	float meshStartX, meshEndX, meshStartY, meshEndY;
	int vertexCount;

	bool hasMesh;
	bool hasSpilt;
	bool meshSpawned;

	UPROPERTY()
		TArray<UOctreeNodes*> childNodes;

	//this will spawn the mesh
	void SpawnPlanetMesh();

private:
	//this will caluclate the rough centre of the mesh
	FVector CalculatePreMeshCentre();
	void SpiltNode();
	//will work out if child nodes have mesh 
	bool ChildNodesGeneratedMesh();

	bool markedForRemoving;

	//this removes all the child nodes of this node
	void RemoveAllChildNodes();

public:
	//this will move all the meshes
	void MoveMeshes(FVector newLocation);
};
