// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineTest.h"


AEngineTest::AEngineTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//set the mesh of this model
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/Ships/Engines/ThrusterBlockout.ThrusterBlockout'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);

	//intilise all variables!
	partWeight = 2000;
	partCentre = FVector(0, 0, -290);
	//engineMaxThrust = 200000.f;
	engineMaxThrust = 800000.f;
	engineCurrentThrust = 0.f;
	engineThrustPoint = FVector(0, 0, -580);
	engineThrustDirection = FVector(0, 0, 1);
	engineMaxFuel = 0.f;
	engineCurrentFuel = 0.f;

	mesh->SetCenterOfMass(partCentre);
	mesh->SetMassOverrideInKg("Mesh", partWeight);
}

//void AEngineTest::BeginPlay()
//{
//}
//
//void AEngineTest::Tick(float DeltaTime)
//{
//}
