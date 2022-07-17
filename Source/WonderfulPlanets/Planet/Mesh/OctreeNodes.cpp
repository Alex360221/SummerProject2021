// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeNodes.h"
#include "PlanetMesh.h"
#include "PlanetController.h"
#include "NoiseGenerator.h"

UOctreeNodes::UOctreeNodes()
{

}

void UOctreeNodes::Init(APlanetController* newPlanetController, UOctreeNodes* newParentNode, int newOctreeLevel, float newMeshStartX, float newMeshEndX, float newMeshStartY, float newMeshEndY, FVector newLocalUp, int newVertexCount, int newOctreeIndex)
{
	//innit lod view distances
	targetDistance.Add(10000000);
	targetDistance.Add(7500000);
	targetDistance.Add(4000000);
	targetDistance.Add(2000000);
	targetDistance.Add(1000000);
	targetDistance.Add(500000);
	targetDistance.Add(100000);


	planetController = newPlanetController;
	parentNode = newParentNode;
	octreeLevel = newOctreeLevel;
	meshStartX = newMeshStartX;
	meshEndX = newMeshEndX;
	meshStartY = newMeshStartY;
	meshEndY = newMeshEndY;
	localUp = newLocalUp;
	vertexCount = newVertexCount;
	octreeIndex = newOctreeIndex;
	//NEED TO WORK OUT A ROUGH LOCATION OF THE MESH CENTRE BEFORE THE MESH IS CREATED SO THAT WE SOMETHING TO WORK OF
	meshCentre = CalculatePreMeshCentre();
	//GLog->Log("Mesh centre . " + FString::FromInt(meshCentre.X) + " , " + FString::FromInt(meshCentre.Y) + " , " + FString::FromInt(meshCentre.Z));

	hasSpilt = false;
	hasMesh = false;
	meshSpawned = false;
	//markedForRemoving = false;

	SpawnPlanetMesh();
}

void UOctreeNodes::Update(FVector playerPos)
{
	float dis = FVector::Dist(playerPos, (meshCentre + planetController->planetCentre));
	//work out if this mesh should spilt
	//first check if its reached the minuim level

	if (hasSpilt)
	{
		//check if this chunk has a mesh and if children all have mesh, remove this mesh
		if (hasMesh && ChildNodesGeneratedMesh() && !markedForRemoving)
		{
			//GLog->Log("RemoveNode!!");
			//can safley remove this mesh
			hasMesh = false;
			meshSpawned = false;
			//add planet mesh for destorying
			planetController->AddPlanetMeshForDestorying(planetMesh);
			planetMesh = nullptr;
		}

		if (octreeLevel < targetDistance.Num())
		{
			if (dis > targetDistance[octreeLevel])
			{
				if (hasMesh && meshSpawned)
				{
					//remove all Nodes
					RemoveAllChildNodes();
					hasSpilt = false;
					markedForRemoving = false;
				}
				else if (!hasMesh && !markedForRemoving)
				{
					//spawn mesh then the nodes will be removed!!
					//GLog->Log("Gen New Mesh!!");
					markedForRemoving = true;
					SpawnPlanetMesh();
				}
			}
		}

		//update all child Nodes
		for (int i = 0; i < childNodes.Num();i++)
		{
			//GLog->Log("Update Nodes" + FString::FromInt(i));
			if (childNodes[i]) { childNodes[i]->Update(playerPos); }
		}
	}
	else
	{
		if (octreeLevel < targetDistance.Num())
		{
			if (dis < targetDistance[octreeLevel])
			{
				//spilt node
//				GLog->Log("Spilt ");
				SpiltNode();
			}
		}
	}
}

void UOctreeNodes::SpawnPlanetMesh()
{
	if (planetController && !hasMesh)
	{
		hasMesh = true;
		meshSpawned = false;
		//		GLog->Log("Spawn Plant Mesh");
		planetMesh = planetController->SpawnChunk(this, localUp, meshStartX, meshEndX, meshStartY, meshEndY);
	}
}

FVector UOctreeNodes::CalculatePreMeshCentre()
{
	FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
	FVector axisB = FVector::CrossProduct(localUp, axisA);
	//work out centre x and centre y
	float range = meshEndX - meshStartX;
	int totalVertices = vertexCount / range;
	//int lowerBoundX = (totalVertices)*meshStartX;
	//int higherBoundX = (totalVertices)*meshEndX;
	int midX = totalVertices * (meshStartX + (range / 2.f));
	//int lowerBoundY = (totalVertices)*meshStartY;
	//int higherBoundY = (totalVertices)*meshEndY;
	int midY = totalVertices * (meshStartY + (range / 2.f));

	//work out mid point of of vaule
	FVector2D percent = FVector2D(midX, midY) / totalVertices;
	FVector pointOnUnitCube = localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
	FVector pointOnUnitSphere = NoiseGenerator::CalculatePointOnPlanet(pointOnUnitCube, planetController->planetRadius, FVector(0,0,0)/*planetController->planetCentre*/);

	return pointOnUnitSphere;
}

void UOctreeNodes::SpiltNode()
{
	if (hasSpilt) { return; }
	//	GLog->Log("Spilt .. " + FString::FromInt(hasMesh));
	hasSpilt = true;
	markedForRemoving = false;
	float halfRange = (meshEndX - meshStartX) / 2.f;

	//create 4 new nodes 
	//UOctreeNodes* newNode = NewObject<UOctreeNodes>();
	childNodes.Add(NewObject<UOctreeNodes>());
	childNodes[0]->Init(planetController, this, octreeLevel + 1, meshStartX, meshStartX + halfRange, meshStartY, meshStartY + halfRange
		, localUp, vertexCount, 0);
	//childNodes.Add(new OctreeNode(planetController, this, octreeLevel + 1, meshStartX, meshStartX + halfRange, meshStartY, meshStartY + halfRange
	//	, localUp, vertexCount, 0));
	
	childNodes.Add(NewObject<UOctreeNodes>());
	childNodes[1]->Init(planetController, this, octreeLevel + 1, meshStartX + halfRange, meshEndX, meshStartY, meshStartY + halfRange
		, localUp, vertexCount, 1);
	//childNodes.Add(new OctreeNode(planetController, this, octreeLevel + 1, meshStartX + halfRange, meshEndX, meshStartY, meshStartY + halfRange
	//	, localUp, vertexCount, 1));
	childNodes.Add(NewObject<UOctreeNodes>());
	childNodes[2]->Init(planetController, this, octreeLevel + 1, meshStartX, meshStartX + halfRange, meshStartY + halfRange, meshEndY
		, localUp, vertexCount, 2);
	//childNodes.Add(new OctreeNode(planetController, this, octreeLevel + 1, meshStartX, meshStartX + halfRange, meshStartY + halfRange, meshEndY
	//	, localUp, vertexCount, 2));
	childNodes.Add(NewObject<UOctreeNodes>());
	childNodes[3]->Init(planetController, this, octreeLevel + 1, meshStartX + halfRange, meshEndX, meshStartY + halfRange, meshEndY
		, localUp, vertexCount, 3);
	//childNodes.Add(new OctreeNode(planetController, this, octreeLevel + 1, meshStartX + halfRange, meshEndX, meshStartY + halfRange, meshEndY
	//	, localUp, vertexCount, 3));
}

bool UOctreeNodes::ChildNodesGeneratedMesh()
{
	int meshGenerated = 0;
	for (int i = 0; i < childNodes.Num();i++)
	{
		UOctreeNodes* currentNode = childNodes[i];
		if (currentNode)
		{

			if (currentNode->hasSpilt)
			{
				if (currentNode->ChildNodesGeneratedMesh()) { meshGenerated += 1; }
			}
			else
			{

				if (currentNode->meshSpawned)
				{
					//GLog->Log("Running.");
					meshGenerated += 1;
				}
			}
		}
		currentNode = nullptr;
	}
	//	GLog->Log("chidl nodes generated  = " + FString::FromInt(meshGenerated));
	if (meshGenerated >= 4) { return true; }
	return false;
}

void UOctreeNodes::RemoveAllChildNodes()
{
	if (childNodes.Num() == 0) { return; }

	for (int i = 0; i < childNodes.Num();i++)
	{
		//GLog->Log("Update Nodes" + FString::FromInt(i));
		UOctreeNodes* currentNode = childNodes[i];
		if (currentNode)
		{
			currentNode->RemoveAllChildNodes();
			planetController->AddPlanetMeshForDestorying(currentNode->planetMesh);
			currentNode->planetMesh = nullptr;
		}
		currentNode = nullptr;
	}
	childNodes.Empty();
}

void UOctreeNodes::MoveMeshes(FVector newLocation)
{
	if (planetMesh) { planetMesh->SetActorLocation(newLocation); }
	for (int i = 0; i < childNodes.Num();i++)
	{
		childNodes[i]->MoveMeshes(newLocation);
	}
}
