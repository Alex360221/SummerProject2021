// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainTextureGenerator.h"
#include <WonderfulPlanets/Planet/Mesh/PlanetMesh.h>
#include <WonderfulPlanets/Lib/Noise/FastNoise.h>
#include <WonderfulPlanets/Planet/Mesh/Texture/ColourPicker.h>
#include <WonderfulPlanets/Planet/Mesh/NoiseGenerator.h>

TerrainTextureGenerator::TerrainTextureGenerator(APlanetMesh* planetMesh, float startPointX, float endPointX, float startPointY, float endPointY, int vertexCount, FVector localUp, float planetRadius, FVector planetCenter)
{
	this->planetMesh = planetMesh;
	this->startPointX = startPointX;
	this->endPointX = endPointX;
	this->startPointY = startPointY;
	this->endPointY = endPointY;
	this->vertexCount = vertexCount;
	this->localUp = localUp;
	this->planetRadius = planetRadius;
	this->planetCenter = planetCenter;
}

TerrainTextureGenerator::~TerrainTextureGenerator()
{
	UE_LOG(LogTemp, Warning, TEXT("Texture gen Done!!"));
}

void TerrainTextureGenerator::DoWork()
{
	//First stage create the colour array 
	TArray<FColor> colourData;

	//float uvScale = 1 / (vertexCount - 1);
	float range = endPointX - startPointX;
	float uvScale = 1 / ((vertexCount - 1) / range);
	//TArray <FColor> VertexColour;

	FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
	FVector axisB = FVector::CrossProduct(localUp, axisA);

	int triIndex = 0;
	int totalVertices = vertexCount / range;
	int lowerBoundX = (totalVertices)*startPointX;
	int higherBoundX = (totalVertices)*endPointX;
	int midX = totalVertices * ((startPointX + range) / 2.f);
	int lowerBoundY = (totalVertices)*startPointY;
	int higherBoundY = (totalVertices)*endPointY;
	int midY = totalVertices * ((startPointY + range) / 2.f);;

	//if (localUp == FVector::UpVector)
	//{
	for (int x = higherBoundX /*+ 3*/; x >= (lowerBoundX /*- 3*/);x--)
	{
		for (int y = (lowerBoundY /*- 3*/); y <= higherBoundY /*+ 3*/;y++)
		{
			int amountOfVertices = vertexCount + 6;
			int verticesMesh_Count = vertexCount + 4;
			//int i = x + y * vertexCount;
			//FVector2D percent = FVector2D(x, y) / (vertexCount - 1);
			FVector2D percent = FVector2D(x, y) / totalVertices;
			//if not this mesh use different local up to calucalte vertex position
			FVector pointOnUnitCube;
			//if (x >= 0 && x < totalVertices && y >= 0 && y < totalVertices)
			//{
				pointOnUnitCube = localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
			//}
			//else
			//{
				//pointOnUnitCube = localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
			//	pointOnUnitCube = CalculatePointOnOtherUnitSphere(x, y, percent, (1.f / totalVertices), /*localUp, */totalVertices);
			//}

			//////////////
			float x2 = pointOnUnitCube.X * pointOnUnitCube.X;
			float y2 = pointOnUnitCube.Y * pointOnUnitCube.Y;
			float z2 = pointOnUnitCube.Z * pointOnUnitCube.Z;
			FVector s;
			s.X = pointOnUnitCube.X * FMath::Sqrt(1.f - (y2 / 2.f) - (z2 / 2.f) + ((y2 * z2) / 3.f));
			s.Y = pointOnUnitCube.Y * FMath::Sqrt(1.f - (x2 / 2.f) - (z2 / 2.f) + ((z2 * x2) / 3.f));
			s.Z = pointOnUnitCube.Z * FMath::Sqrt(1.f - (x2 / 2.f) - (y2 / 2.f) + ((x2 * y2) / 3.f));
			//GLog->Log("v");
			FVector pointOnUnitSphere = s;
			/// //////////
			FVector2D uv = percent;//CalculatePointOnPlanet(pointOnUnitCube/*pointOnUnitSphere*/);
			//pointOnUnitSphere = shapeGenerator->CalculatePointOnPlanet(pointOnUnitSphere);
			pointOnUnitSphere = NoiseGenerator::CalculatePointOnPlanet(pointOnUnitCube, planetRadius, planetCenter);


			//if (x >= lowerBoundX - 3 && x <= higherBoundX + 3 && y >= lowerBoundY - 3 && y <= higherBoundY + 3)
			//{//In the mesh so calaculate the colour for this spot!!
				//colourData.Add(InLoopCalculations(x, y, totalVertices, axisA, axisB));
				if (x < lowerBoundX /*- 2*/) { colourData.Add(FColor::Green); }
				else if (y < lowerBoundY /*- 2*/) { colourData.Add(FColor::Orange); }
				else if (x > higherBoundX /*+ 2*/) { colourData.Add(FColor::Purple); }
				else if (y > higherBoundY /*+ 2*/) { colourData.Add(FColor::Yellow); }
				else { colourData.Add(ColourPicker::CalculateColour(pointOnUnitSphere, planetCenter, planetRadius)); }
				//colourData.Add(ColourPicker::CalculateColour(pointOnUnitSphere, planetCenter, planetRadius));
			//}
			//else { colourData.Add(FColor::Blue); }
		}
	}

	//send data back to planet mesh
	planetMesh->colourData = colourData;

	//let it know its got all the data so it can create the texture
	planetMesh->generatedMasterImage = true;

	colourData.Empty();
}

FVector TerrainTextureGenerator::CalculatePointOnOtherUnitSphere(int x, int y, FVector2D percent, float sVaule, int totalVertexCount)
{
	//work out what localup vector to use
	FVector localLocalUp = localUp;
	FVector2D localPercent = percent;
	FQuat rotation = FQuat();
	FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
	FVector axisB = FVector::CrossProduct(localUp, axisA);

	if (localUp == FVector::UpVector)
	{//up mesh
		if (x < 0) { return PointOnOtherUnitSphere(FVector::LeftVector, FVector2D(percent.Y, sVaule)); }
		if (x > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::RightVector, FVector2D(1 - percent.Y, sVaule)); }
		//if (x < 0) { return PointOnOtherUnitSphere(FVector::RightVector, FVector2D(1 - percent.Y, sVaule)); }
		//if (x > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::LeftVector, FVector2D(percent.Y, sVaule)); }
		if (y < 0) { return PointOnOtherUnitSphere(FVector::ForwardVector, FVector2D(1 - sVaule, 1 - percent.X)); }
		if (y > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::BackwardVector, FVector2D(sVaule, 1 - percent.X)); }
		//if (y > totalVertexCount - 1 ) { return PointOnOtherUnitSphere(FVector::ForwardVector, FVector2D(1 - sVaule, 1 - percent.X)); }
		//if (y < 0) { return PointOnOtherUnitSphere(FVector::BackwardVector, FVector2D(sVaule, 1 - percent.X)); }
	}
	else if (localUp == FVector::DownVector)
	{//Down mesh
		if (x < 0) { return PointOnOtherUnitSphere(FVector::RightVector, FVector2D(1 - percent.Y, 1 - sVaule)); }
		if (x > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::LeftVector, FVector2D(percent.Y, 1 - sVaule)); }
		if (y < 0) { return PointOnOtherUnitSphere(FVector::ForwardVector, FVector2D(sVaule, percent.X)); }
		if (y > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::BackwardVector, FVector2D(1 - sVaule, percent.X)); }
	}
	else if (localUp == FVector::LeftVector)
	{//left mesh
		if (x < 0) { return PointOnOtherUnitSphere(FVector::ForwardVector, FVector2D(1 - percent.Y, 1 - sVaule)); }
		if (x > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::BackwardVector, FVector2D(percent.Y, 1 - sVaule)); }
		if (y < 0) { return PointOnOtherUnitSphere(FVector::UpVector, FVector2D(sVaule, percent.X)); }
		if (y > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::DownVector, FVector2D(1 - sVaule, percent.X)); }
	}
	else if (localUp == FVector::RightVector)
	{//right Mesh
		if (x < 0) { return PointOnOtherUnitSphere(FVector::BackwardVector, FVector2D(percent.Y, sVaule)); }
		if (x > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::ForwardVector, FVector2D(1 - percent.Y, sVaule)); }
		if (y < 0) { return PointOnOtherUnitSphere(FVector::UpVector, FVector2D(1 - sVaule, 1 - percent.X)); }
		if (y > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::DownVector, FVector2D(sVaule, 1 - percent.X)); }

	}
	else if (localUp == FVector::ForwardVector)
	{//forward Mesh
		if (x < 0) { return PointOnOtherUnitSphere(FVector::DownVector, FVector2D(percent.Y, sVaule)); }
		if (x > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::UpVector, FVector2D(1 - percent.Y, sVaule)); }
		if (y < 0) { return PointOnOtherUnitSphere(FVector::RightVector, FVector2D(1 - sVaule, 1 - percent.X)); }
		if (y > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::LeftVector, FVector2D(sVaule, 1 - percent.X)); }
	}
	else if (localLocalUp == FVector::BackwardVector)
	{
		if (x < 0) { return PointOnOtherUnitSphere(FVector::UpVector, FVector2D(1 - percent.Y, 1 - sVaule)); }
		if (x > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::DownVector, FVector2D(percent.Y, 1 - sVaule)); }
		if (y < 0) { return PointOnOtherUnitSphere(FVector::RightVector, FVector2D(sVaule, percent.X)); }
		if (y > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::LeftVector, FVector2D(1 - sVaule, percent.X)); }
	}
	return localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
}

FVector TerrainTextureGenerator::PointOnOtherUnitSphere(FVector localLocalUp, FVector2D localPercent)
{
	FVector localAxisA = FVector(localLocalUp.Y, localLocalUp.Z, localLocalUp.X);
	FVector localAxisB = FVector::CrossProduct(localLocalUp, localAxisA);
	return localLocalUp + (localPercent.X - .5f) * 2 * localAxisA + (localPercent.Y - .5f) * 2 * localAxisB;
}

float TerrainTextureGenerator::CalculatePointOnPlanet(FVector pointOnUnitSphere)
{	
	//nosie vaules
	FastNoise* fn = new FastNoise();
	fn->SetNoiseType(FastNoise::Perlin);
	fn->SetSeed(74874);

	pointOnUnitSphere.Normalize();
	float frequency = 3.3;
	float elevation = 0;

	FVector point = pointOnUnitSphere;

	point = point * frequency + planetCenter;
	float v = fn->GetNoise(point.X, point.Y, point.Z);
	//float noiseValue = (v + 1) * .5f * 1;
	//noiseValue = FMath::Max<float>(0, noiseValue - 1.2);

	//elevation = noiseValue * 0.3;
	elevation = planetRadius * (1 + elevation);

	FVector pointOnPlanet =	pointOnUnitSphere * elevation;

	float oDis = FVector::Dist(pointOnPlanet, planetCenter);
	float aHeight = oDis - (planetRadius);
	//if (aHeight < 0.f) { aHeight = 0.0f; }
	//oDis /= 100.f;
	float maxHeight = 1000.f;	//this will be a set height for now
	//take the max int vaule for a colour which 244 and divide it by that max disatance then times by aHeight
	float s = (255.f / maxHeight) * aHeight;

	return v * planetRadius;
}
