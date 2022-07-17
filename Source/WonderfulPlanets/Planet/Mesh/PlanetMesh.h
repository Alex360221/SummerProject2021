// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PlanetMesh.generated.h"

UCLASS()
class WONDERFULPLANETS_API APlanetMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* terrainMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* materialInterface;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* terrainMat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector meshMidPoint;
	FVector localUp;
	UPROPERTY()
		class UOctreeNodes* node;
	int vertexCount;
	float planetRadius;
	//float meshStartX, meshStartY, meshEndX, meshEndY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float meshStartX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float meshStartY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float meshEndX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float meshEndY;


	bool generatedMesh;
	bool createdMesh;

	TArray<FVector> vertices;
	TArray<int> triangles;
	TArray<FVector2D> UVs;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> Tangents;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//call once all variables have been set
	void Start();
	

	//Image variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* masterTerrainImage;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FColor> colourData;
	bool generateMasterImage, generatedMasterImage, generatingMasterImage, createMasterImage;

	void CalculateMeshVaules();
	void GenerateMesh();
	void GenerateImage();

	void DestroyMesh();
private:
	bool destroySelf;


};
