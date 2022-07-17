// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingTest3.h"


ABuildingTest3::ABuildingTest3()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/SpaceCentre/SatelliteBase.SatelliteBase'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);

	mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mesh 1"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object2(TEXT("StaticMesh'/Game/Objects/SpaceCentre/SatelliteDish.SatelliteDish'"));
	assset = object2.Object;
	mesh1->SetStaticMesh(assset);

	mesh1->SetupAttachment(mesh);
	mesh1->SetRelativeLocation(FVector(0, 0, 6000));
	mesh1->SetRelativeRotation(FRotator(45, 0, 0));
}


//

