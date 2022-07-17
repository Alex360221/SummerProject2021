// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateMesh.h"
#include "PlanetMesh.h"
#include "NoiseGenerator.h"

CreateMesh::CreateMesh(APlanetMesh* planetMesh, float startPointX, float endPointX, float startPointY, float endPointY, int vertexCount, FVector localUp, float planetRadius, FVector planetCenter)
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

CreateMesh::~CreateMesh()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Mesh Done!!"));
}

void CreateMesh::DoWork()
{
	TArray<FVector> vertices;
	TArray<int> triangles;
	TArray<FVector2D> UVs;

	TArray<FVector> vertices_Full;
	TArray<int> triangles_Full;
	TArray<FVector2D> UVs_Full;
	TArray<int> normalsIndices;	//this stores which normal should be used for the mesh

	FVector midPoint;

	bool flipTrianglulation;
	//float uvScale = 1 / (vertexCount - 1);
	float range = endPointX - startPointX;

	TArray <FColor> VertexColour;

	FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
	FVector axisB = FVector::CrossProduct(localUp, axisA);

	int triIndex = 0;
	int totalVertices = vertexCount / range;
	float uvScaleMult = 1 / range;
	float uvScale = 1 / ((vertexCount /*+ 1*/) / range);

	//float uvScale = 1 / totalVertices;
	int lowerBoundX = (totalVertices)*startPointX;
	int higherBoundX = (totalVertices)*endPointX;
	int midX = totalVertices * ((startPointX + range) / 2.f);
	int lowerBoundY = (totalVertices)*startPointY;
	int higherBoundY = (totalVertices)*endPointY;
	int midY = totalVertices * ((startPointY + range) / 2.f);;

	float uvScaleMultX = 1.f, uvScaleMultY = 1.f;
	float uvScaleAddX = 0.f, uvScaleAddY = 0.f;

	if (startPointX < 0.5) { uvScaleMultX = uvScaleMult; uvScaleMultY = uvScaleMult; /*uvScaleAddX = -1.f*/;
	}
	else { uvScaleAddX = -1; }

	if (startPointY < 0.5) { uvScaleMultY = uvScaleMult; uvScaleMultX = uvScaleMult;/* uvScaleAddY = -1.f*/;
	}
	else { uvScaleAddY = -1; }

	//if (startPointX < 0.5 || startPointY < 0.5) { uvScaleMultX = uvScaleMult; uvScaleMultY = uvScaleMult; }

	//uvScaleMultX = uvScaleMult; 
	//uvScaleMultY = uvScaleMult;

	//else { uvScaleAddX = -0.5; uvScaleAddY = -0.5;	}
	//if (startPointX >=0.5){ uvScaleAddX = -0.5; }
	//if (startPointY >=0.5){ uvScaleAddY = -0.5; }

	//for (int x = -3; x < vertexCount + 3;x++)
	for (int x = (lowerBoundX /*- 3*/); x <= higherBoundX /*+ 3*/;x++)
	{
		//for (int y = -3; y < vertexCount + 3;y++)
		for (int y = (lowerBoundY /*- 3*/); y <= higherBoundY /*+ 3*/;y++)
		{
			int amountOfVertices = vertexCount + 6;
			int verticesMesh_Count = vertexCount + 1;
			flipTrianglulation = false;
			//int i = x + y * vertexCount;
			//FVector2D percent = FVector2D(x, y) / (vertexCount - 1);
			FVector2D percent = FVector2D(x, y) / totalVertices;
			//if not this mesh use different local up to calucalte vertex position
			FVector pointOnUnitCube;
			//if (x >= 0 && x <= totalVertices && y >= 0 && y <= totalVertices)
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
			//pointOnUnitSphere = pointOnUnitSphere * planetRadius;


			flipTrianglulation = ShouldFlipTriangluation(x, y, /*localUp, */totalVertices);
			//if (x >= lowerBoundX /*- 2*/ && x <= higherBoundX -1/*+ 1*/ && y >= lowerBoundY /*- 2*/ && y <= higherBoundY -1/*+ 1*/)
			//{//in mesh section so add to normal arrays
				int i = vertices.Add(pointOnUnitSphere);
				UVs.Add(FVector2D(((x * uvScale) - startPointX) * uvScaleMult, ((y * uvScale) - startPointY) * uvScaleMult));
				if (x != higherBoundX /*-1*//*+ 1*/ && y != higherBoundY /*-1*//*+ 1*/)
				{
					Triangulation(i, verticesMesh_Count, &triangles, flipTrianglulation);
				}

				//sets the mid point
				if (x == midX && y == midY)
				{
					midPoint = vertices[i];
					//if (meshActor)
					//{
					//	meshActor->meshMidPoint = vertices[i];
					//}
				}
			//}

			//UVs_Full.Add(uv);
			//int i_F = vertices_Full.Add(pointOnUnitSphere);
			//if (x != higherBoundX + 2 && y != higherBoundY + 2)
			//{
			//	Triangulation(i_F, amountOfVertices, &triangles_Full, flipTrianglulation);
			//}
		}
	}



	planetMesh->vertices = vertices;
	planetMesh->triangles = triangles;
	planetMesh->UVs = UVs;

	planetMesh->meshMidPoint = midPoint;

	planetMesh->generatedMesh = true;
}

void CreateMesh::Triangulation(int index, int verticesInLine, TArray<int>* outArray, bool flip)
{
	if (flip != true)
	{
		outArray->Add(index);
		outArray->Add(index + verticesInLine + 1);
		outArray->Add(index + verticesInLine);

		outArray->Add(index);
		outArray->Add(index + 1);
		outArray->Add(index + verticesInLine + 1);
	}
	else
	{
		outArray->Add(index);
		outArray->Add(index + 1);
		outArray->Add(index + verticesInLine);

		outArray->Add(index + 1);
		outArray->Add(index + verticesInLine + 1);
		outArray->Add(index + verticesInLine);
	}
}

FVector CreateMesh::CalculatePointOnOtherUnitSphere(int x, int y, FVector2D percent, float sVaule, int totalVertexCount)
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
		if (y < 0) { return PointOnOtherUnitSphere(FVector::ForwardVector, FVector2D(1 - sVaule, 1 - percent.X)); }
		if (y > totalVertexCount - 1) { return PointOnOtherUnitSphere(FVector::BackwardVector, FVector2D(sVaule, 1 - percent.X)); }
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

FVector CreateMesh::PointOnOtherUnitSphere(FVector localLocalUp, FVector2D localPercent)
{
	FVector localAxisA = FVector(localLocalUp.Y, localLocalUp.Z, localLocalUp.X);
	FVector localAxisB = FVector::CrossProduct(localLocalUp, localAxisA);
	return localLocalUp + (localPercent.X - .5f) * 2 * localAxisA + (localPercent.Y - .5f) * 2 * localAxisB;
}

bool CreateMesh::ShouldFlipTriangluation(int x, int y, int totalVertexCount)
{
	if (x < 0) { return true; }
	if (x >= totalVertexCount - 1) { return true; }
	if (y < 0) { return true; }
	if (y >= totalVertexCount - 1) { return true; }
	return false;
}
