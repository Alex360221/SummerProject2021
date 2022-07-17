// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BasePlayer.generated.h"

UCLASS()
class WONDERFULPLANETS_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* springArm;
	UPROPERTY()
		UStaticMeshComponent* springArmAttachPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cameraDistance;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FOR TESTING PURPOSE
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool spawningPlanets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool inShip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABaseShip* ship;

	class APlanetController* nearestPlanet;
private:		

	class ASolarSystemController* currentSolarSystem;

	float timer;

	//camera Movement
	void TurnAtRate(float value);
	void LookUpRate(float value);
	void ClickTurnAtRate(float value);
	void ClickLookAtRate(float value);
	void ScrollWheelUp();
	void ScrollWheelDown();

	//movement controller - passes movement into ship on a fixed tick update
	void ShipMovementController();
	//movement controller - passes movemnt into the current building NOT fixed tick update
	void BuildingMovementController();

	AActor* ClickedOnObject();

	//controls the sensitivity of movement
	float buildingCentreMovementSensitivity = 1.f;
	//movement functions
	void LeftShiftDown(){ leftShiftDown = true; }
	void LeftShiftUp(){ leftShiftDown = false; }
	bool leftShiftDown = false;

	void LeftCTRLDown(){ leftCTRLDown = true; }
	void LeftCTRLUp(){ leftCTRLDown = false; }
	bool leftCTRLDown = false;

	void WKeyDown(){ wKeyDown = true; }
	void WKeyUp(){ wKeyDown = false; }
	bool wKeyDown = false;

	void SKeyDown(){ sKeyDown = true; }
	void SKeyUp(){ sKeyDown = false; }
	bool sKeyDown = false;

	void AKeyDown(){ aKeyDown = true; }
	void AKeyUp(){ aKeyDown = false; }
	bool aKeyDown = false;

	void DKeyDown() { dKeyDown = true; }
	void DKeyUp() { dKeyDown = false; }
	bool dKeyDown = false;

	void QKeyDown() { qKeyDown = true; }
	void QKeyUp() { qKeyDown = false; }
	bool qKeyDown = false;

	void EKeyDown() { eKeyDown = true; }
	void EKeyUp(){ eKeyDown = false; }
	bool eKeyDown = false;

	void ZKeyDown() { zKeyDown = true; }
	void ZKeyUp() { zKeyDown = false; }
	bool zKeyDown = false;

	void XKeyDown() { xKeyDown = true; }
	void XKeyUp() { xKeyDown = false; }
	bool xKeyDown = false;

	void PeriodDown();
	void CommaDown();

	void RightMouseDown() { rightMouseDown = true; }
	void RightMouseUp() { rightMouseDown = false; }
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool rightMouseDown = false;
private:
	void LeftMouseDown() { leftMouseDown = true; }
	void LeftMouseUp() { leftMouseDown = false; }
	bool leftMouseDown = false;

	void MiddleMouseDown() { middleMouseDown = true; }
	void MiddleMouseUp() { middleMouseDown = false; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool middleMouseDown = false;

private:
	void ToggleShipBuilding();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool inShipBuildingCentre = false;
	bool inShipToStart = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseShipBuildingCentre* currentShipBuildingCentre;
private:
	//this will return the mouse location in the world
	FVector GetMouseInWorld();
	//this will return the impact point
	FVector GetMouseImpactPoint();
};
