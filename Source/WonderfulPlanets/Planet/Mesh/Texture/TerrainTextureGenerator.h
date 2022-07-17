// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Runtime/Core/Public/Async/AsyncWork.h"

/**
 * 
 */
class WONDERFULPLANETS_API TerrainTextureGenerator : public FNonAbandonableTask
{
	friend class FAutoDeleteAsyncTask <TerrainTextureGenerator>;
public:
	TerrainTextureGenerator(class APlanetMesh* planetMesh, float startPointX, float endPointX, float startPointY, float endPointY, int vertexCount, FVector localUp,
		float planetRadius, FVector planetCenter);

	~TerrainTextureGenerator();

	/*This function is executed when we tell our task to execute*/
	void DoWork();

	float startPointX;
	float endPointX;
	float startPointY;
	float endPointY;
	int vertexCount;
	FVector localUp;
	float planetRadius;
	FVector planetCenter;
	class APlanetMesh* planetMesh;


	FVector CalculatePointOnOtherUnitSphere(int x, int y, FVector2D percent, float sVaule, int totalVertexCount);
	FVector PointOnOtherUnitSphere(FVector localLocalUp, FVector2D localPercent);
	//this will work out the point on planet with noise
	float CalculatePointOnPlanet(FVector pointOnUnitSphere);

	/*This function is needed from the API of the engine.
My guess is that it provides necessary information
about the thread that we occupy and the progress of our task*/
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(TerrainTextureGenerator, STATGROUP_ThreadPoolAsyncTasks);
	}
};
