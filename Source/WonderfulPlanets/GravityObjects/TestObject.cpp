// Fill out your copyright notice in the Description page of Project Settings.


#include "TestObject.h"
#include <WonderfulPlanets/Planet/GravityCalculator.h>

// Sets default values
ATestObject::ATestObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMesh'/Game/Objects/Ships/BasicShip.BasicShip'
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/Ships/BasicShip.BasicShip'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);

	mesh->SetupAttachment(RootComponent);
	SetRootComponent(mesh);
	forwardSpeed = 1000;
}

// Called when the game starts or when spawned
void ATestObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mesh->SetEnableGravity(false);
	mesh->SetSimulatePhysics(true);

	GetVelocity();
		
	FVector forward = GetActorForwardVector() * forwardSpeed;
	FVector g = GravityCalculator::CalculateGravityForObject(100, GetActorLocation(), FVector(0,0,0));
	mesh->SetPhysicsLinearVelocity(g + forward, true);
	
}

