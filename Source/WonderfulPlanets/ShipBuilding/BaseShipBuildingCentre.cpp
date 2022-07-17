// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShipBuildingCentre.h"
#include "WonderfulPlanets/GravityObjects/PartsList.h"
#include "WonderfulPlanets/GravityObjects/BaseShipPart.h"
#include <WonderfulPlanets/GravityObjects/ShipPart.h>
#include "Arrow.h"
#include "BaseGizmo.h"
#include "Circle.h"
#include <WonderfulPlanets/Saving/ShipSaveData.h>
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseShipBuildingCentre::ABaseShipBuildingCentre()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/Buildings/DryDockBlockout.DryDockBlockout'"));
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	clickedPartMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("Clicked Mat"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>object2(TEXT("Material'/Game/Materials/BuildingCentre/ClickedPart_Mat.ClickedPart_Mat'"));
	clickedPartMaterial = object2.Object;

	defaultPartMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("Default Mat"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>object3(TEXT("Material'/Game/Materials/BuildingCentre/DefaultPart_Mat.DefaultPart_Mat'"));
	defaultPartMaterial = object3.Object;

	SetRootComponent(mesh);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	cameraDistance = 10000.f;

	camera->SetupAttachment(RootComponent);
	//springArm->SetupAttachment(RootComponent);
	//springArm->TargetArmLength = cameraDistance;
	//springArm->SetRelativeRotation(FRotator(45.f, 0.f, 0.f));
	//camera->SetupAttachment(springArm);
	camera->SetRelativeLocation(FVector(0, 0, 1000));

}

// Called when the game starts or when spawned
void ABaseShipBuildingCentre::BeginPlay()
{
	Super::BeginPlay();
	SpawnGizmo();
}

// Called every frame
void ABaseShipBuildingCentre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseShipBuildingCentre::SpawnShipPart(int partID)
{
	GLog->Log("Spawning Part: " + FString::FromInt(partID));

	ABaseShipPart* newPart = GetWorld()->SpawnActor<ABaseShipPart>
		(PartsList::GetShipPart(partID), GetActorLocation() + (FQuat(GetActorRotation()) * FVector(0,0,1000)), GetActorRotation());
	FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, true);
	newPart->AttachToActor(this, attachRules);
	newPart->partID = partID;
	shipParts.Add(newPart);
}

void ABaseShipBuildingCentre::LeftClick(AActor* clickedObject, FVector impactPoint)
{
	//is it a ship part?
	ABaseShipPart* sp = Cast<ABaseShipPart>(clickedObject);
	if (sp != nullptr && sp != currentClickedPart && !leftMouseDown)
	{
		//we have a new ship part clicked
		//set old mesh to not have highlight
		if (currentClickedPart) 
		{ 
			currentClickedPart->mesh->SetMaterial(0, defaultPartMaterial); 
			/*currentClickedPart->mesh->SetRenderCustomDepth(false);*/ 
			rightClick = false;
			ToggleArrows(false);
		}
		currentClickedPart = sp;
		rightClick = false;
		GLog->Log("Clicked Actor : " + FString(currentClickedPart->GetDebugName(currentClickedPart)));
		//enable highlight on new ship aprt
		//currentClickedPart->mesh->SetRenderCustomDepth(true);
		currentClickedPart->mesh->SetMaterial(0, clickedPartMaterial);
		// set arrows
		SetArrowsPos(currentClickedPart->GetActorLocation() + (currentClickedPart->GetActorQuat() * currentClickedPart->partCentre));
		ScaleArrows(currentClickedPart->arrowScale);
		SetCirclesPos(currentClickedPart->GetActorLocation() + (currentClickedPart->GetActorQuat() * currentClickedPart->partCentre));
		ScaleCircles(currentClickedPart->arrowScale);
		/*ShowArrows();*/		
		allowObjectMovement = true;
		if (gizmoSetting == 0) { ToggleArrows(true); }
		else if (gizmoSetting == 1) { ToggleCircles(true); }
	}
	//not ship part is it arrows? - if so get mosue movement
	ABaseGizmo* gizmo = Cast<ABaseGizmo>(clickedObject);
	if (gizmo)
	{
		if (!currentGizmo) 
		{ 
			currentGizmo = gizmo; 
			mouseHitImpactPoint = impactPoint /*- currentClickedPart->GetActorLocation()*/;
		}
		//get mouse movement 
		
		allowObjectMovement = true;
		GLog->Log("Arrow selected ");
	}
}

void ABaseShipBuildingCentre::LeftClickMovePart(FVector mouseWorldPos, FVector impactPoint)
{		
	
	
	if (gizmoSetting == 0)
	{
		FVector localPos = GetActorQuat().Inverse() * (mouseWorldPos - GetActorLocation());
		if (lastMouseLocalPos == FVector(0, 0, 0)) { lastMouseLocalPos = localPos; return; }
		FVector travel = localPos - lastMouseLocalPos;

		if (currentGizmo) { MoveArrowSetAxis(GetAxis(travel, currentGizmo->direction) * 10, currentGizmo->direction); }
		if (lastMouseLocalPos != FVector(0, 0, 0)) { lastMouseLocalPos = localPos; }
	}
	else if (gizmoSetting == 1)
	{
		if (currentGizmo)
		{
			FVector localPos = GetActorQuat().Inverse() * (mouseWorldPos - GetActorLocation());
			FVector dir = (mouseWorldPos - camera->GetComponentLocation());
			dir.Normalize();

			//get distance between impact point and camera
			float dis = FVector::Dist(camera->GetComponentLocation(), mouseHitImpactPoint);
			FVector newMousePos = dir * dis;
			newMousePos = (newMousePos + camera->GetComponentLocation()) - currentClickedPart->GetActorLocation();
			//FVector travel = localPos - lastMouseLocalPos;

			if (lastMouseLocalPos == FVector(0, 0, 0)) { lastMouseLocalPos = newMousePos; return; }

			FVector2D dir2d = FVector2D(Get2DAxis(newMousePos,currentGizmo->movementDirection
				, currentClickedPart->GetActorQuat() * FQuat(currentGizmo->relativeRotation)))
				- FVector2D(0, 0);
			float angle = FMath::Atan2(dir2d.Y, dir2d.X);

			if (lastAngle == 0) { lastAngle = angle; return; }
			FQuat newRotation = FQuat(currentGizmo->movementDirection, angle - lastAngle);

			if (inverse) { newRotation = newRotation.Inverse(); }
			//UE_LOG(LogTemp, Warning, TEXT("The angle is: %f"), angle);

			if (lastAngle != angle)
			{
				lastAngle = angle;
				currentClickedPart->AddActorLocalRotation(newRotation)/* - lastMouseRotation*/;
				SetCirclesRotation(currentClickedPart->GetActorQuat());
			}		
			if (lastMouseLocalPos != FVector(0, 0, 0)) { lastMouseLocalPos = newMousePos; }
		}
	}
}

void ABaseShipBuildingCentre::LeftClickStop()
{
	allowObjectMovement = false;
	currentGizmo = nullptr;
	lastMouseLocalPos = FVector(0, 0, 0);
	lastAngle = 0;
}

void ABaseShipBuildingCentre::RightMouseToggle(bool toggle)
{
	
	if (toggle != rightMouseHolder)
	{ 
		GLog->Log("RIGHT MOUSE");
		rightMouseHolder = toggle;
		rightMouseCount += 1;
	}
	//right click should only toggle if rightmousecount >= 2 and we've clicked on an ship part
	if (rightMouseCount >= 2)
	{
		rightMouseCount = 0;
		if (rightClick)
		{
			GLog->Log("RIGHT MOUSE =============== FALSE");
			rightClick = false;
			if (gizmoSetting == 0) { ToggleArrows(false); }
			else if (gizmoSetting == 1) { ToggleCircles(false); }
		}
		else
		{
			GLog->Log("RIGHT MOUSE =============== TRUE");
			rightClick = true;
			if (gizmoSetting == 0) { ToggleArrows(true); }
			else if (gizmoSetting == 1) { ToggleCircles(true); }
		}
	}
}

void ABaseShipBuildingCentre::SpawnGizmo()
{
	for (int i = 0; i < 3;i++)
	{
		AArrow* newArrow = GetWorld()->SpawnActor<AArrow>(GetActorLocation(), GetActorRotation());
		if (newArrow)
		{
			newArrow->gizmoDirection = i;
			newArrow->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			newArrow->SetActorRelativeLocation(FVector(0, 0, 500));
			newArrow->RotateGizmo();
			gizmoArrows.Add(newArrow);
		}

		ACircle* newCircle = GetWorld()->SpawnActor<ACircle>(GetActorLocation(), GetActorRotation());
		if (newCircle)
		{
			newCircle->gizmoDirection = i;
			newCircle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			newCircle->SetActorRelativeLocation(FVector(0, 0, 500));
			newCircle->RotateGizmo();
			gizmoCircles.Add(newCircle);
		}
	}
	HideArrows();
	HideCircles();
}

void ABaseShipBuildingCentre::ToggleArrows(bool toggle)
{
	if (!toggle) 
	{ 
		HideArrows();
		ToggleShipPartsCollisions(true);
		rightClick = false;
		GLog->Log("Hiding arrows");
	}
	else
	{
		if (rightClick /*&& allowObjectMovement*/ && currentClickedPart) 
		{
			ShowArrows(); 
			ToggleShipPartsCollisions(false);
		GLog->Log("showing arrows");
		}
		else { GLog->Log("Something not right"); }
	}
}

void ABaseShipBuildingCentre::ToggleCircles(bool toggle)
{
	if (!toggle)
	{
		HideCircles();
		ToggleShipPartsCollisions(true);
		rightClick = false;
		GLog->Log("Hiding arrows");
	}
	else
	{
		if (rightClick /*&& allowObjectMovement*/ && currentClickedPart)
		{
			ShowCircles();
			ToggleShipPartsCollisions(false);
			GLog->Log("showing arrows");
		}
		else { GLog->Log("Something not right"); }
	}
}

void ABaseShipBuildingCentre::HideArrows()
{
	for (int i = 0; i < gizmoArrows.Num();i++)
	{
		gizmoArrows[i]->mesh->SetVisibility(false);
		gizmoArrows[i]->mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		gizmoArrows[i]->mesh->SetRenderCustomDepth(false);
	}
}

void ABaseShipBuildingCentre::HideCircles()
{
	for (int i = 0; i < gizmoCircles.Num();i++)
	{
		gizmoCircles[i]->mesh->SetVisibility(false);
		gizmoCircles[i]->mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//gizmoCircles[i]->mesh->SetRenderCustomDepth(false);
	}
}

void ABaseShipBuildingCentre::ShowArrows()
{
	for (int i = 0; i < gizmoArrows.Num();i++)
	{
		gizmoArrows[i]->mesh->SetVisibility(true);
		gizmoArrows[i]->mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		gizmoArrows[i]->mesh->SetRenderCustomDepth(true);
	}
}

void ABaseShipBuildingCentre::ShowCircles()
{
	for (int i = 0; i < gizmoCircles.Num();i++)
	{
		gizmoCircles[i]->mesh->SetVisibility(true);
		gizmoCircles[i]->mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//gizmoCircles[i]->mesh->SetRenderCustomDepth(true);
	}
}

void ABaseShipBuildingCentre::ScaleArrows(float scale)
{
	for (int i = 0; i < gizmoArrows.Num();i++)
	{
		gizmoArrows[i]->SetActorScale3D(FVector(scale, scale, scale));
	}
}

void ABaseShipBuildingCentre::ScaleCircles(float scale)
{
	for (int i = 0; i < gizmoCircles.Num();i++)
	{
		gizmoCircles[i]->SetActorScale3D(FVector(scale, scale, scale));
	}
}

void ABaseShipBuildingCentre::SetArrowsPos(FVector pos)
{
	for (int i = 0; i < gizmoArrows.Num();i++)
	{
		gizmoArrows[i]->SetActorLocation(pos);
	}
}

void ABaseShipBuildingCentre::SetCirclesPos(FVector pos)
{
	for (int i = 0; i < gizmoCircles.Num();i++)
	{
		gizmoCircles[i]->SetActorLocation(pos);
	}
}

void ABaseShipBuildingCentre::SetCirclesRotation(FQuat rot)
{
	for (int i = 0; i < gizmoCircles.Num();i++)
	{
		gizmoCircles[i]->SetActorRotation(rot * FQuat(gizmoCircles[i]->relativeRotation));
	}
}

void ABaseShipBuildingCentre::MoveArrowSetAxis(float movement, FVector axis)
{
	if (currentGizmo && allowObjectMovement)
	{
		currentClickedPart->SetActorRelativeLocation(SnappingGridConvertLocalPosition((GetActorQuat().Inverse() * (currentClickedPart->GetActorLocation() - GetActorLocation()))
			+ (axis * (movement * gridSize))));

		SetArrowsPos(currentClickedPart->GetActorLocation() + (currentClickedPart->GetActorQuat() * currentClickedPart->partCentre));
		SetCirclesPos(currentClickedPart->GetActorLocation() + (currentClickedPart->GetActorQuat() * currentClickedPart->partCentre));
	}
}

FVector2D ABaseShipBuildingCentre::Get2DAxis(FVector pos, FVector axis, FQuat gizmoRotation)
{
	FVector defualtAxis = axis;
	//gets the direction between the camera and the shi part
	FVector dir = GetActorQuat().Inverse() * ((camera->GetComponentLocation() - GetActorLocation()) -
		(currentClickedPart->GetActorLocation() - GetActorLocation()));
	dir.Normalize();

	if ((dir.X >= 0.5 || dir.X <= -0.5) && dir.Y >= -0.7 && dir.Y <= 0.7 && dir.Z >= -0.7 && dir.Z <= 0.7)
	{
		//GLog->Log("x area!!!");
		inverse = true;
		return FVector2D(pos.Y, pos.Z);
	}
	else if ((dir.Y >= 0.5 || dir.Y <= -0.5) && dir.X >= -0.7 && dir.X <= 0.7 && dir.Z >= -0.7 && dir.Z <= 0.7)
	{
		//GLog->Log("Y area!!!");
		inverse = false;
		return FVector2D(pos.X, pos.Z);
	}
	else if ((dir.Z >= 0.5 || dir.Z <= -0.5) && dir.Y >= -0.7 && dir.Y <= 0.6 && dir.X >= -0.7 && dir.X <= 0.7)
	{
		//GLog->Log("Z area!!!");
		inverse = false;
		return FVector2D(pos.X, pos.Y);
	}
	else {
		//GLog->Log("No area");
		inverse = false;
		return FVector2D(0, 0);
	}
}

float ABaseShipBuildingCentre::GetAxis(FVector pos, FVector axis)
{
	if (axis.X != 0) { return pos.X; }
	else if (axis.Y != 0) { return pos.Y; }
	else if (axis.Z != 0) { return pos.Z; }
	return 0.0f;
}

void ABaseShipBuildingCentre::MoveObject(float value, FVector movementDirection)
{
	if (allowObjectMovement && currentClickedPart)
	{
		value = value * 10;
		movementDirection = camera->GetComponentQuat() * movementDirection;
		//UE_LOG(LogTemp, Warning, TEXT("MovemntDirection is : %s"), *movementDirection.ToString());
		//currentClickedPart->SetActorLocation(currentClickedPart->GetActorLocation() + (movementDirection * value));
	}
}

void ABaseShipBuildingCentre::ToggleShipPartsCollisions(bool toggle)
{
	for (int i = 0; i < shipParts.Num();i++)
	{
		if(toggle){ shipParts[i]->mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); }
		else { shipParts[i]->mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); }
	}
}

FVector ABaseShipBuildingCentre::SnappingGridConvertLocalPosition(FVector localPos)
{
	return FVector(SnappingGridConvertFloat(localPos.X),
		SnappingGridConvertFloat(localPos.Y), SnappingGridConvertFloat(localPos.Z));
}

void ABaseShipBuildingCentre::SaveShip()
{
	if (UShipSaveData* saveShip = Cast<UShipSaveData>(UGameplayStatics::CreateSaveGameObject(UShipSaveData::StaticClass())))
	{
		//set the data
		//saveShip->shipParts = CreateShipPartsArray();
		for (int i = 0; i < shipParts.Num();i++)
		{
			saveShip->partID.Add(shipParts[i]->partID);
			FVector t = GetActorQuat().Inverse()* (shipParts[i]->GetActorLocation() - GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Ship part spawn location is: %s"), *t.ToString());
			if (t.Z < 0) { t.Z = t.Z * -1; }
			UE_LOG(LogTemp, Warning, TEXT("Ship part spawn location is: %s"), *t.ToString());
			saveShip->partPosition.Add(t);
			saveShip->partRotation.Add(shipParts[i]->GetActorQuat() - GetActorQuat());
			saveShip->attachedTo.Add(0);
		}

		//save the data immediately - MIGHT NEED CHANGING FOR ASYCHRONOUS
		if (UGameplayStatics::SaveGameToSlot(saveShip, FString("ship1"), 0))
		{
			GLog->Log("SAVE SHIP SUCCESSFULLY");
		}
	}
}

TArray<class UShipPart*> ABaseShipBuildingCentre::CreateShipPartsArray()
{
	TArray<UShipPart*> rShipParts;

	for (int i = 0; i < shipParts.Num();i++)
	{
		UShipPart* newPart = NewObject<UShipPart>();
		newPart->Init(shipParts[i]->partID, shipParts[i]->GetActorLocation() - GetActorLocation(),
			shipParts[i]->GetActorQuat(), 0);
		rShipParts.Add(newPart);
	}

	return rShipParts;
}

