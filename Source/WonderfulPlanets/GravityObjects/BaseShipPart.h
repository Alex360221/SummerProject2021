// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseShipPart.generated.h"

UCLASS()
class WONDERFULPLANETS_API ABaseShipPart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseShipPart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//shared variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mesh;

	int partID = 0;
	float partWeight;
	FVector partCentre;
	int attachment;
	float arrowScale;

	//Toggles Collision
	void ToggleCollision(bool collision);
	//Toggles Physics
	void TogglePhysics(bool physics);


};
