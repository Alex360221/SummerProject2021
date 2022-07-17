// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGizmo.h"

// Sets default values
ABaseGizmo::ABaseGizmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	gizmoAxisX = CreateDefaultSubobject<UMaterialInterface>(TEXT("Gizmo X Axis"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>object0(TEXT("MaterialInstanceConstant'/Game/Objects/Buildings/BuildingCentre/GizmoAxis_X_Inst.GizmoAxis_X_Inst'"));
	gizmoAxisX = object0.Object;

	gizmoAxisY = CreateDefaultSubobject<UMaterialInterface>(TEXT("Gizmo Y Axis"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>object1(TEXT("MaterialInstanceConstant'/Game/Objects/Buildings/BuildingCentre/GizmoAxis_Y_Inst.GizmoAxis_Y_Inst'"));
	gizmoAxisY = object1.Object;

	gizmoAxisZ = CreateDefaultSubobject<UMaterialInterface>(TEXT("Gizmo Z Axis"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>object2(TEXT("MaterialInstanceConstant'/Game/Objects/Buildings/BuildingCentre/GizmoAxis_Z_Inst.GizmoAxis_Z_Inst'"));
	gizmoAxisZ = object2.Object;
}

// Called when the game starts or when spawned
void ABaseGizmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGizmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseGizmo::RotateGizmo()
{
	if (gizmoDirection == 0) 
	{ 
		direction = (FVector(0, 0, 1)); 
		mesh->SetMaterial(0, gizmoAxisZ);
	}
	if (gizmoDirection == 1)
	{
		SetActorRelativeRotation(FRotator(0, 0, 90));
		direction = (FVector(0, 1, 0));
		mesh->SetMaterial(0, gizmoAxisY);
	}
	else if (gizmoDirection == 2)
	{
		SetActorRelativeRotation(FRotator(90, 0, 0));
		direction = (FVector(1, 0, 0));
		mesh->SetMaterial(0, gizmoAxisX);
	}
}

