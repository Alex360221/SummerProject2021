// Fill out your copyright notice in the Description page of Project Settings.


#include "CockpitStaticMehses.h"

// Sets default values
ACockpitStaticMehses::ACockpitStaticMehses()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/Ships/Cockpits/CockpitBlockout.CockpitBlockout'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);
	mesh->SetVisibility(false);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	cockpitMeshes.Add(1, mesh);
}

// Called when the game starts or when spawned
void ACockpitStaticMehses::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACockpitStaticMehses::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

