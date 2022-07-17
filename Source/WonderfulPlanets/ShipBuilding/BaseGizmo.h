// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGizmo.generated.h"

UCLASS()
class WONDERFULPLANETS_API ABaseGizmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGizmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UStaticMeshComponent* mesh;

	int gizmoDirection;
	FVector direction;
	FRotator relativeRotation;
	FVector movementDirection;

	UMaterialInterface* gizmoAxisX;
	UMaterialInterface* gizmoAxisY;
	UMaterialInterface* gizmoAxisZ;


	//this is to be called after the circle direction has been set, will rotate the circle accordinly 
	void RotateGizmo();

};
