// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayer.h"
#include <WonderfulPlanets/Planet/Mesh/PlanetController.h>
#include "GameFramework/CharacterMovementComponent.h"
#include <WonderfulPlanets/Planet/GravityCalculator.h>
#include <WonderfulPlanets/GravityObjects/BaseShip.h>
#include <WonderfulPlanets/ShipBuilding/BaseShipBuildingCentre.h>
#include <WonderfulPlanets/Galaxy/SolarSystemController.h>

#include "DrawDebugHelpers.h"

// Sets default values
ABasePlayer::ABasePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	spawningPlanets = false;
	inShip = false;

	springArmAttachPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spring Arm Attach Point"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));

	cameraDistance = 1000.f;

	springArmAttachPoint->SetupAttachment(RootComponent);
	springArm->SetupAttachment(springArmAttachPoint);
	springArm->TargetArmLength = cameraDistance;
	springArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	//springArm->bEnableCameraLag = true;
	//springArm->CameraLagSpeed = 2;
	//springArm->CameraLagMaxDistance = 1.5f;

	//springArm->bEnableCameraRotationLag = true;
	//springArm->CameraRotationLagSpeed = 4;
	//springArm->CameraLagMaxTimeStep = 1;
	springArm->bDoCollisionTest = false;
	springArm->bUsePawnControlRotation = true;

	camera->SetupAttachment(springArm,USpringArmComponent::SocketName);
	
	//timer
	timer = 0.f;

}

// Called when the game starts or when spawned
void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	//turns off gravity for player
	GetCharacterMovement()->GravityScale = 0.f;
	
	//Spawn Planet Controller
	if (spawningPlanets)
	{
		//spawn the glaxy controller
		currentSolarSystem = GetWorld()->SpawnActor<ASolarSystemController>(FVector(0, 0, 0), FRotator(0, 0, 0));
		if (currentSolarSystem)
		{
			//init solar system variables
			currentSolarSystem->player = this;

			//start the solar system controller eg spawning planets;
			currentSolarSystem->Start();
		}
		/*APlanetController* newPC = GetWorld()->SpawnActor<APlanetController>(FVector(0, 0, 0), FRotator(0, 0, 0));
		if (newPC)
		{
			newPC->playerRef = this;
			newPC->Start();
			planets.Add(newPC);
		}*/
	}	
	if (inShip && ship)
	{
		SetActorEnableCollision(false);
		//attach player to ship
		AttachToActor(ship,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//ship->nearestPlanet = planets[0];
		APlayerController* pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			GLog->Log("Set Camera to ship camera");
			//pc->SetViewTarget(ship);
		}
	//	K2_AttachRootComponentToActor(ship);
		AttachToActor(ship, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

// Called every frame
void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timer += DeltaTime;
	if (timer >= 0.1)
	{
		timer = 0.f;
		//if (!inShip && spawningPlanets)
		//{
		//	UMovementComponent* movementCom = GetMovementComponent();
		//	if (movementCom)
		//	{
		//		//move player
		//		FVector g = GravityCalculator::CalculateGravityForObject(100, GetActorLocation(), planets);
		//		movementCom->Velocity = (g + movementCom->Velocity);
		//	}
		//}

		//update ship movement
		ShipMovementController();
	}
	BuildingMovementController();
}

// Called to bind functionality to input
void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Camera Controls
	PlayerInputComponent->BindAxis("Turn", this, &ABasePlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasePlayer::LookUpRate);
	PlayerInputComponent->BindAction("ScrollWheelUp", IE_Pressed, this, &ABasePlayer::ScrollWheelUp);
	PlayerInputComponent->BindAction("ScrollWheelDown", IE_Pressed, this, &ABasePlayer::ScrollWheelDown);

	PlayerInputComponent->BindAction("LeftShift", IE_Pressed, this, &ABasePlayer::LeftShiftDown);
	PlayerInputComponent->BindAction("LeftShift", IE_Released, this, &ABasePlayer::LeftShiftUp);
	PlayerInputComponent->BindAction("LeftCTRL", IE_Pressed, this, &ABasePlayer::LeftCTRLDown);
	PlayerInputComponent->BindAction("LeftCTRL", IE_Released, this, &ABasePlayer::LeftCTRLUp);
	PlayerInputComponent->BindAction("WKey", IE_Pressed, this, &ABasePlayer::WKeyDown);
	PlayerInputComponent->BindAction("WKey", IE_Released, this, &ABasePlayer::WKeyUp);
	PlayerInputComponent->BindAction("SKey", IE_Pressed, this, &ABasePlayer::SKeyDown);
	PlayerInputComponent->BindAction("SKey", IE_Released, this, &ABasePlayer::SKeyUp);
	PlayerInputComponent->BindAction("AKey", IE_Pressed, this, &ABasePlayer::AKeyDown);
	PlayerInputComponent->BindAction("AKey", IE_Released, this, &ABasePlayer::AKeyUp);
	PlayerInputComponent->BindAction("DKey", IE_Pressed, this, &ABasePlayer::DKeyDown);
	PlayerInputComponent->BindAction("DKey", IE_Released, this, &ABasePlayer::DKeyUp);
	PlayerInputComponent->BindAction("QKey", IE_Pressed, this, &ABasePlayer::QKeyDown);
	PlayerInputComponent->BindAction("QKey", IE_Released, this, &ABasePlayer::QKeyUp);
	PlayerInputComponent->BindAction("EKey", IE_Pressed, this, &ABasePlayer::EKeyDown);
	PlayerInputComponent->BindAction("EKey", IE_Released, this, &ABasePlayer::EKeyUp);
	PlayerInputComponent->BindAction("ZKey", IE_Pressed, this, &ABasePlayer::ZKeyDown);
	PlayerInputComponent->BindAction("ZKey", IE_Released, this, &ABasePlayer::ZKeyUp);
	PlayerInputComponent->BindAction("XKey", IE_Pressed, this, &ABasePlayer::XKeyDown);
	PlayerInputComponent->BindAction("XKey", IE_Released, this, &ABasePlayer::XKeyUp);
	PlayerInputComponent->BindAction("Period", IE_Pressed, this, &ABasePlayer::PeriodDown);
	PlayerInputComponent->BindAction("Comma", IE_Pressed, this, &ABasePlayer::CommaDown);

	PlayerInputComponent->BindAction("IKey", IE_Pressed, this, &ABasePlayer::ToggleShipBuilding);

	PlayerInputComponent->BindAction("RightMouse", IE_Pressed, this, &ABasePlayer::RightMouseDown);
	PlayerInputComponent->BindAction("RightMouse", IE_Released, this, &ABasePlayer::RightMouseUp);
	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &ABasePlayer::LeftMouseDown);
	PlayerInputComponent->BindAction("LeftMouse", IE_Released, this, &ABasePlayer::LeftMouseUp);
	PlayerInputComponent->BindAction("MiddleMouse", IE_Pressed, this, &ABasePlayer::MiddleMouseDown);
	PlayerInputComponent->BindAction("MiddleMouse", IE_Released, this, &ABasePlayer::MiddleMouseUp);

	//Period
}
//camera controls
void ABasePlayer::TurnAtRate(float value)
{ 
	if (middleMouseDown) { ClickTurnAtRate(value); }
	if (leftMouseDown && inShipBuildingCentre && currentShipBuildingCentre) 
	{
		currentShipBuildingCentre->MoveObject(value, FVector(0,1,0));
	}
	//if (!leftMouseDown && inShipBuildingCentre && currentShipBuildingCentre) { currentShipBuildingCentre->LeftClickStop(); }
	ABasePlayer::AddControllerYawInput(value);		
}

void ABasePlayer::LookUpRate(float value)
{
	if (middleMouseDown) { ClickLookAtRate(value); }
	if (leftMouseDown && inShipBuildingCentre && currentShipBuildingCentre) 
	{ currentShipBuildingCentre->MoveObject(value, FVector(0,0,-1)); }
//	if (!leftMouseDown && inShipBuildingCentre && currentShipBuildingCentre) { currentShipBuildingCentre->LeftClickStop(); }
	ABasePlayer::AddControllerPitchInput(value);
}
void ABasePlayer::ClickTurnAtRate(float value)
{
	if (inShipBuildingCentre && currentShipBuildingCentre) { currentShipBuildingCentre->TurnAtRate(value); }
}
void ABasePlayer::ClickLookAtRate(float value)
{
	if (inShipBuildingCentre && currentShipBuildingCentre) { currentShipBuildingCentre->LookAtRate(value); }
}
void ABasePlayer::ScrollWheelUp()
{
	if (cameraDistance + 100 <= 10000 && inShip)
	{
		cameraDistance += 100;
		springArm->TargetArmLength = cameraDistance;
	}
	if (buildingCentreMovementSensitivity + 0.1 <= 2.f && inShipBuildingCentre) { buildingCentreMovementSensitivity += 0.1; }
}
void ABasePlayer::ScrollWheelDown()
{
	if (cameraDistance - 100 >= 1000 && inShip)
	{
		cameraDistance -= 100;
		springArm->TargetArmLength = cameraDistance;
	}
	if (buildingCentreMovementSensitivity - 0.1 >= 0.1f && inShipBuildingCentre) { buildingCentreMovementSensitivity -= 0.1; }
}
void ABasePlayer::ShipMovementController()
{
	if (inShip && ship)
	{
		if (leftShiftDown) { ship->ThrottleUp(); }
		if (leftCTRLDown) { ship->ThrottleDown(); }
		if (wKeyDown) { ship->PitchUp(); }
		if (sKeyDown) { ship->PitchDown(); }
		if (aKeyDown) { ship->YawLeft(); }
		if (dKeyDown) { ship->YawRight(); }
		if (qKeyDown) { ship->RollLeft(); }
		if (eKeyDown) { ship->RollRight(); }
		if (zKeyDown) { ship->MoveUp(); }
		if (xKeyDown) { ship->MoveDown(); }
	}
}

void ABasePlayer::BuildingMovementController()
{
	if (inShipBuildingCentre && currentShipBuildingCentre)
	{
		if (wKeyDown) { currentShipBuildingCentre->MoveX(10 * buildingCentreMovementSensitivity); }
		if (sKeyDown) { currentShipBuildingCentre->MoveX(-10 * buildingCentreMovementSensitivity); }
		if (aKeyDown) { currentShipBuildingCentre->MoveY(-10 * buildingCentreMovementSensitivity); }
		if (dKeyDown) { currentShipBuildingCentre->MoveY(10 * buildingCentreMovementSensitivity); }
		if (zKeyDown) { currentShipBuildingCentre->MoveZ(10 * buildingCentreMovementSensitivity); }
		if (xKeyDown) { currentShipBuildingCentre->MoveZ(-10 * buildingCentreMovementSensitivity); }

		if (leftMouseDown) 
		{ 
			currentShipBuildingCentre->LeftClick(ClickedOnObject(), GetMouseImpactPoint());
			currentShipBuildingCentre->LeftClickMovePart(GetMouseInWorld(), GetMouseImpactPoint());
		}
		else { currentShipBuildingCentre->LeftClickStop(); }
		currentShipBuildingCentre->LeftMouseToggle(leftMouseDown);
		currentShipBuildingCentre->RightMouseToggle(rightMouseDown);

	}
}

AActor* ABasePlayer::ClickedOnObject()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	FVector Location, Direction;
	PC->DeprojectMousePositionToWorld(Location, Direction);	//mouse location
	FVector end = Location + (Direction * 10000);	//the ned point of the line trace using direction
	FHitResult hitResult;
	//DrawDebugLine(GetWorld(), Location, end, FColor::Green, true, 1, 0,1000);
	
	//UE_LOG(LogTemp, Warning, TEXT("Line start pos: %s"), *Location.ToString());
	if(PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, hitResult))
	//if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, end, ECC_Visibility))
	{
		AActor* hitActor;
		hitActor = Cast<AActor>(hitResult.Actor);
		if (hitActor)
		{
		//	GLog->Log("Clicked Actor : " + FString(hitActor->GetDebugName(hitActor)));
			return hitActor;
		}
		GLog->Log("Non hit");
	}

	return nullptr;
}

void ABasePlayer::PeriodDown()
{
	if (inShip && ship) { ship->FullThrottle(); }
}

void ABasePlayer::CommaDown()
{
	if (inShip && ship) { ship->NoThrottle(); }
}

void ABasePlayer::ToggleShipBuilding()
{
	if (!inShipBuildingCentre)
	{
		if (nearestPlanet)
		{
			currentShipBuildingCentre = nearestPlanet->shipBuilder;
			if (currentShipBuildingCentre)
			{
				
				APlayerController* pc = Cast<APlayerController>(GetController());
				if (pc)
				{
					GLog->Log("SHIP BUILDING CENTRE en");		
					AttachToActor(currentShipBuildingCentre, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					pc->SetViewTarget(currentShipBuildingCentre);
					inShipBuildingCentre = true;
					inShipToStart = inShip;
					inShip = false;
				}
			}
		}
	}
	else
	{	
		APlayerController* pc = Cast<APlayerController>(GetController());
		if (pc)
		{
			GLog->Log("SHIP BUILDING CENTRE dis");
			inShip = inShipToStart;
			inShipToStart = false;
			pc->SetViewTarget(this);
			AttachToActor(ship, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			inShipBuildingCentre = false;
			
		}
	}
}

FVector ABasePlayer::GetMouseInWorld()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	FVector Location, Direction;
	PC->DeprojectMousePositionToWorld(Location, Direction);	//mouse location
	return Location;
}

FVector ABasePlayer::GetMouseImpactPoint()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	FVector Location, Direction;
	PC->DeprojectMousePositionToWorld(Location, Direction);	//mouse location
	FVector end = Location + (Direction * 10000);	//the ned point of the line trace using direction
	FHitResult hitResult;
	if (PC->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery_MAX, true, hitResult))
		//if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, end, ECC_Visibility))
	{
		return hitResult.ImpactPoint;
	}
	return FVector();
}

