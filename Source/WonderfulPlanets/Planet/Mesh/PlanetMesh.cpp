// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetMesh.h"
#include "CreateMesh.h"
#include "Texture/TerrainTextureGenerator.h"
#include <WonderfulPlanets/Planet/Mesh/OctreeNodes.h>

// Sets default values
APlanetMesh::APlanetMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshMidPoint = FVector(0, 0, 0);
	localUp = FVector(0, 0, 0);
	vertexCount = 0;

	generateMasterImage = false;
	generatedMasterImage = false;
	generatingMasterImage = false;
	createMasterImage = false;

	terrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain Mesh"));
	terrainMesh->SetupAttachment(RootComponent);
	SetRootComponent(terrainMesh);
//	terrainMesh->SetBoundsScale(10.0f);
	//
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>Material1(TEXT("MaterialInstanceConstant'/Game/Materials/Planet/BasicPlanet_Mat_Inst.BasicPlanet_Mat_Inst'"));
	materialInterface = Material1.Object;

	//set up collsion so it can be hit by ship line trace
	terrainMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void APlanetMesh::BeginPlay()
{
	Super::BeginPlay();
	terrainMat = UMaterialInstanceDynamic::Create(materialInterface, this);
	UMaterialInstanceDynamic* testy = UMaterialInstanceDynamic::Create(materialInterface, this);
	GLog->Log(testy->GetName());
}

// Called every frame
void APlanetMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (generatedMasterImage)
	{
		if (!createMasterImage)
		{
			createMasterImage = true;
			if (destroySelf) 
			{ 
				Destroy(); 
				return;
			}//this chunk should be destroyed so no need to gen mesh
			//create image....
			GenerateImage();

			//now generate mesh
			CalculateMeshVaules();
		}		
	}
	if (generatedMesh)
	{
		if (!createdMesh)
		{
			if (destroySelf) 
			{ 
				Destroy(); 
				return;
			}
			createdMesh = true;
			//GLog->Log("Created data and stuff");
			TArray<FVector> normals_Full;
			TArray<FProcMeshTangent> tangents_Full;
			TArray <FColor> VertexColour;
			terrainMesh->CreateMeshSection(0, vertices, triangles, normals_Full, UVs, VertexColour, tangents_Full, true);
			terrainMesh->SetMaterial(0, terrainMat);
			//terrainMesh->lig
			vertices.Empty();
			triangles.Empty();
			UVs.Empty();
			if (node) 
			{
			//	node->meshCentre = meshMidPoint; 
				node->meshSpawned = true;
			}
			//terrainMat->SetScalarParameterValue("SwitchMethod", textureType);
		}
	}
	if (destroySelf)
	{
		Destroy();
	}
}

void APlanetMesh::Start()
{
	//call thread task to create texture
	(new FAutoDeleteAsyncTask<TerrainTextureGenerator>(
		this, meshStartX, meshEndX, meshStartY, meshEndY, vertexCount*5, localUp, planetRadius, FVector(0, 0, 0))
		)->StartBackgroundTask();
	//generatedMasterImage = true;
}

void APlanetMesh::CalculateMeshVaules()
{
	////call thread task to create mesh
	(new FAutoDeleteAsyncTask<CreateMesh>(
		this, meshStartX, meshEndX, meshStartY, meshEndY, vertexCount, localUp, planetRadius, FVector(0, 0, 0))
		)->StartBackgroundTask();
}

void APlanetMesh::GenerateMesh()
{
}

void APlanetMesh::GenerateImage()
{
	if (colourData.Num() == 0) { return; }
	int imageRes = (vertexCount *5) +1/*+ 6*//* * 10*/; //(vertexCount * 4) + 2;
	//UTexture2D* masterTerrainImage;
	masterTerrainImage = UTexture2D::CreateTransient(imageRes, imageRes, PF_R8G8B8A8);
	masterTerrainImage->NeverStream = false;
	masterTerrainImage->SRGB = 0;
	//FCreateTexture2DParameters textureParamerters = FCreateTexture2DParameters();
	//highResTexture = FImageUtils::CreateTexture2D(imageRes, imageRes, colourData,this,"texure",
	//	EObjectFlags::RF_Dynamic, textureParamerters);

	FTexture2DMipMap& mip = masterTerrainImage->PlatformData->Mips[0];
	void* mipdata = mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(mipdata, colourData.GetData(), imageRes * imageRes * 4);
	mip.BulkData.Unlock();
	masterTerrainImage->UpdateResource();
	terrainMat->SetTextureParameterValue("TerrainTexture", masterTerrainImage);
	

	colourData.Empty();
}

void APlanetMesh::DestroyMesh()
{
	//if mesh has been calculated destory mesh stright away
	//if not tell it to destroy after mesh gen been done
	if (createdMesh) 
	{
		Destroy(); 
	}
	else { destroySelf = true; }
}



