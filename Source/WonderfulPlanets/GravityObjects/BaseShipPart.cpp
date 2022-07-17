// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShipPart.h"

// Sets default values
ABaseShipPart::ABaseShipPart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	mesh->SetupAttachment(RootComponent);
	SetRootComponent(mesh);
	mesh->SetEnableGravity(false);
	mesh->SetSimulatePhysics(false);
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	partWeight = 1.f;
	partCentre = FVector(0, 0, 0);
	arrowScale = 2.f;
	mesh->SetMassOverrideInKg("Mesh", partWeight);
}

// Called when the game starts or when spawned
void ABaseShipPart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseShipPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseShipPart::ToggleCollision(bool collision)
{
	if(collision){ mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); }
	else { mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); }
}

void ABaseShipPart::TogglePhysics(bool physics)
{
	mesh->SetSimulatePhysics(physics);
}

