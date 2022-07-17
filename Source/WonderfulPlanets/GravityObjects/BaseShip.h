// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <WonderfulPlanets/GravityObjects/BaseShipPart.h>
#include "BaseShip.generated.h"

UCLASS()
class WONDERFULPLANETS_API ABaseShip : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseShip(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mesh;
	class APlanetController* nearestPlanet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float throttle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shipSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shipAttuiteSeaLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shipAttuiteTerrainLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float shipMass = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector shipCentreOfMass = FVector(0, 0, 0);

	FVector currentVelocity = FVector(0, 0, 0);

//movement methods
	class ASolarSystemController* solarSystemController;
	//controls if the ship or planets move - true = ship, false  = planets
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool moveSelf = true;
	//checks to see if the movement method is different from last tick
	bool lastMoveSelf = moveSelf;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrottleUp();
	void ThrottleDown();
	void PitchUp();
	void PitchDown();
	void YawLeft();
	void YawRight();
	void RollLeft();
	void RollRight();
	void MoveUp();
	void MoveDown();
	void FullThrottle() { throttle = throttleMax; }
	void NoThrottle() { throttle = 0; }


private:
	float timmer;
	float timmerB;
	float throttleMax;

	float turnSensitvity;

	//function to calculate the speed of the ship
	void UpdateShipSpeed(FVector newShipPosistion, float time);
	//FVector lastShipPosistion;
	//calculates the height of the ship above the sea level
	void UpdateShipAttuiteSeaLevel(FVector newShipPosistion);
	//calculates the height of the ship above the terrain
	void UpdateShipAttuiteTerrainLevel(FVector newShipPosistion);
	//calculates the atmosphere pressure on the ship
	float CalculateAtompsherePressure();

	//Ship parts
	class ACockpitStaticMehses* cockpitStaticMeshes;
	//stores the info on the parts
	TArray<class UShipPart*> shipPartsInfo;
	//stores all the ship parts
	TArray<class ABaseShipPart*> shipParts;
	//stores all the engines in an array for quicker acess
	TArray<class ABaseShipPartEngine*> shipEngines;
	//this will store the attachment info key defualt attachment point from shipPartIndo and index is shiparts array index
	TMap<int, int> attachmentInfo;
	//this will build the ship from the array of ship parts
	void BuildShip();
	//this will spilt the ship parts up into sperate arrays for easier access later
	void SpiltShip(class ABaseShipPart* part);
	//this will turn on collisions and physics on all parts
	void TurnOnCollisionsAndPhysics();
	//this will calcualte the intial ship weight
	FVector CalculateIntialCentreOfMass();
	//this will times mass by postion on each axis
	FVector MassPosition(FVector RelativePosition, float mass);
	//this will calcaulte the mass position for the points in the array
	float MassPositionForArray(TMap<float, float>* points);
};
