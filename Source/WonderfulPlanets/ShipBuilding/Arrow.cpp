// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object2(TEXT("StaticMesh'/Game/Objects/Buildings/BuildingCentre/GizmoArrow.GizmoArrow'"));
	UStaticMesh* assset = object2.Object;
	mesh->SetStaticMesh(assset);

	mesh->SetupAttachment(RootComponent);
	gizmoDirection = 0;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
		
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}