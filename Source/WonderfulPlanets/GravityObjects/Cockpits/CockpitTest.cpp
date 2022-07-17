// Fill out your copyright notice in the Description page of Project Settings.


#include "CockpitTest.h"

ACockpitTest::ACockpitTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//sets the mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/Ships/Cockpits/CockpitBlockout.CockpitBlockout'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);

	partWeight = 2000;
	partCentre = FVector(0, 0, 130);
	arrowScale = 2.f;

	mesh->SetCenterOfMass(partCentre);
	mesh->SetMassOverrideInKg("Mesh", partWeight);
}

