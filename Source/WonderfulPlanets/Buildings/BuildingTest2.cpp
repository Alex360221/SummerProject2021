// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingTest2.h"


ABuildingTest2::ABuildingTest2()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/SpaceCentre/Building.Building'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);
}
