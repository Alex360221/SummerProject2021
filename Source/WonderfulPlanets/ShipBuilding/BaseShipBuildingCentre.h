// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"
#include "BaseShipBuildingCentre.generated.h"

UCLASS()
class WONDERFULPLANETS_API ABaseShipBuildingCentre : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseShipBuildingCentre();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* springArm;
	float cameraDistance;

	//spawns the specifc ship part
	UFUNCTION(BlueprintCallable, Category = "Spawn")
		void SpawnShipPart(int partID);
private:
	TArray<class ABaseShipPart*> shipParts;

	
public:

//camera controls
	//turn camera controls
	void TurnAtRate(float value) { camera->SetRelativeRotation(camera->GetRelativeRotation() + FRotator(0, value, 0)); }
	void LookAtRate(float value)
	{
		value *= -1;
		float pitch = camera->GetRelativeRotation().Pitch + value;
		if (pitch < 89.f && pitch > -89.f) { camera->SetRelativeRotation(camera->GetRelativeRotation() + FRotator(value, 0, 0)); }
	}

	//moves the camera
	void MoveX(float value){ camera->SetRelativeLocation(camera->GetRelativeLocation() + (FQuat(camera->GetRelativeRotation()) * FVector(value, 0, 0))); }
	void MoveY(float value){ camera->SetRelativeLocation(camera->GetRelativeLocation() + (FQuat(camera->GetRelativeRotation()) * FVector(0, value, 0))); }
	void MoveZ(float value){ camera->SetRelativeLocation(camera->GetRelativeLocation() + (FVector(0, 0, value))); }


//Actions when parts are clicked on...
	//proccess the left click
	void LeftClick(AActor* clickedObject, FVector impactPoint);
	//this will pass in the mouse location in world space and move the selected ship part
	void LeftClickMovePart(FVector mouseWorldPos, FVector impactPoint);

	//this is to be called when the left click is realsed 
	void LeftClickStop();
	//this will toggle the right click variable
	void RightMouseToggle(bool toggle);
private:
	bool rightClick = false;
	bool rightMouseHolder = false;
	int rightMouseCount = 0;

	FVector mouseHitImpactPoint = FVector(0, 0, 0);

	class UMaterialInterface* clickedPartMaterial;
	class UMaterialInterface* defaultPartMaterial;

	class ABaseShipPart* currentClickedPart = nullptr;

	TArray<class AArrow*> gizmoArrows;
	TArray<class ACircle*> gizmoCircles;
	//spawns the 3 arrows, circles 
	void SpawnGizmo();
	//this will toggle the arrows
	void ToggleArrows(bool toggle);
	//this will toggle the circles
	void ToggleCircles(bool toggle);
	//this will hide the arrow meshes
	void HideArrows();
	//this will hide the circle meshes
	void HideCircles();
	//this will show the arrow meshes
	void ShowArrows();
	//this will show the circle meshes
	void ShowCircles();
	//this will scale the arrows
	void ScaleArrows(float scale);
	//this will scale the circles
	void ScaleCircles(float scale);
	//sets the arrows location
	void SetArrowsPos(FVector pos);
	//sets the circles location
	void SetCirclesPos(FVector pos);
	//sets the circles roation
	void SetCirclesRotation(FQuat rot);
	//Moves arrows along set axis
	void MoveArrowSetAxis(float movement, FVector axis);

	//used for setting the gizmo setting from the ui
	UFUNCTION(BlueprintCallable)
		void SetGizmoSetting(int setting) { 
		gizmoSetting = setting; 
		if (gizmoSetting == 0) { ToggleArrows(true); ToggleCircles(false);}
		else if (gizmoSetting == 1) { ToggleCircles(true); ToggleArrows(false);}
	}

	class ABaseGizmo* currentGizmo;
	int gizmoSetting = 1;
	bool allowObjectMovement = false;
	bool leftMouseDown = false;
	FVector lastMouseLocalPos = FVector(0, 0, 0);

	float lastAngle = 0.f;
	bool inverse = false;

	//this will return the vaules on the axis. 0,0,1 will return 1,1,0
	FVector2D Get2DAxis(FVector pos, FVector axis, FQuat gizmoRotation);
	//this will return the float vaule of the axis
	float GetAxis(FVector pos, FVector axis);

public:
	void LeftMouseToggle(bool toggle) { leftMouseDown = toggle; }

	void MoveObject(float value, FVector movementDirection);

private:
	//this will toggle all the ship parts collisions
	void ToggleShipPartsCollisions(bool toggle);
public:

//SNAPPING GRID
	float gridSize = 3.0f;
	//this will convert the local position onto the snapping grid pos
	FVector SnappingGridConvertLocalPosition(FVector localPos);
	//this will convert a float into the snapping grid
	float SnappingGridConvertFloat(float value){ return FMath::RoundHalfFromZero(value / gridSize) * gridSize; }


//SAVING THE SHIP
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveShip();
private:
	//this will return an array with all the ship parts in
	class TArray<class UShipPart*> CreateShipPartsArray();
};
