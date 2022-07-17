// Fill out your copyright notice in the Description page of Project Settings.


#include "Circle.h"

// Sets default values
ACircle::ACircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//MaterialInstanceConstant'/Game/Objects/Buildings/BuildingCentre/GizmoAxis_X_Inst.GizmoAxis_X_Inst'
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object2(TEXT("StaticMesh'/Game/Objects/Buildings/BuildingCentre/GizmoFullCircle.GizmoFullCircle'"));
	UStaticMesh* assset = object2.Object;
	mesh->SetStaticMesh(assset);

	mesh->SetupAttachment(RootComponent);
	gizmoDirection = 0;
}

// Called when the game starts or when spawned
void ACircle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ACircle::RotateGizmo()
{
	if (gizmoDirection == 0) 
	{
		direction = (FVector(-1, 0, 0)); 
		movementDirection = (FVector(0, -1, 0));	
		mesh->SetMaterial(0, gizmoAxisX);
	}
	if (gizmoDirection == 1)
	{
		SetActorRelativeRotation(FRotator(0, 0, 90));
		relativeRotation = FRotator(0, 0, 90);
		direction = (FVector(0, 0, -1));
		movementDirection = (FVector(0, 0, -1));
		mesh->SetMaterial(0, gizmoAxisZ);
	}
	else if (gizmoDirection == 2)
	{
		SetActorRelativeRotation(FRotator(0, 90, 0));
		relativeRotation = FRotator(0, 90, 0);
		direction = (FVector(0, -1, 0));
		movementDirection = (FVector(-1, 0, 0));
		mesh->SetMaterial(0, gizmoAxisY);
	}
}

