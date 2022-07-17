// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingTest1.h"

ABuildingTest1::ABuildingTest1()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/SpaceCentre/Launchpad.Launchpad'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);
}
