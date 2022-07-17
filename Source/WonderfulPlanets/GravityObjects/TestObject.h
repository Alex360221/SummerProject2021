// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestObject.generated.h"

UCLASS()
class WONDERFULPLANETS_API ATestObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestObject(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class APlanetController*> planets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float forwardSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
