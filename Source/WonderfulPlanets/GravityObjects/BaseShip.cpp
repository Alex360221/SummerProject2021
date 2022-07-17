// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"
#include "Math/UnrealMathUtility.h"
#include <WonderfulPlanets/Planet/GravityCalculator.h>
#include <WonderfulPlanets/Planet/DragCalculator.h>
#include "Kismet/KismetMathLibrary.h"
#include <WonderfulPlanets/Planet/Mesh/PlanetController.h>
#include <WonderfulPlanets/Player/BasePlayer.h>
#include <WonderfulPlanets/Galaxy/SolarSystemController.h>
#include "ShipPart.h"
#include "PartsList.h"
#include "Cockpits/CockpitStaticMehses.h"
#include "BaseShipPartEngine.h"
#include <WonderfulPlanets/Saving/ShipSaveData.h>
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseShip::ABaseShip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, (TEXT("Mesh")));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/Ships/BasicShip.BasicShip'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>object1(TEXT("StaticMesh'/Game/Objects/Cube.Cube'"));
	
	UStaticMesh* assset = object1.Object;
	mesh->SetStaticMesh(assset);

	mesh->SetupAttachment(RootComponent);
	SetRootComponent(mesh);
	mesh->SetEnableGravity(false);
	mesh->SetSimulatePhysics(true);

	
	throttle = 0.f;
	throttleMax = 100.f;
	turnSensitvity = 0.5f;

	//lastShipPosistion = GetActorLocation();
	
}

// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();

	if (UShipSaveData* loadedShip = Cast<UShipSaveData>(UGameplayStatics::LoadGameFromSlot(FString("ship1"), 0)))
	{
		//loading the ship was succesful so set data
		GLog->Log("SUCCESFULLY LOADED SHIP");
		
		for (int i = 0; i < loadedShip->partID.Num();i++)
		{
			UShipPart* newPart = NewObject<UShipPart>();
			newPart->Init(loadedShip->partID[i], loadedShip->partPosition[i],
				loadedShip->partRotation[i], loadedShip->attachedTo[i]);
			shipPartsInfo.Add(newPart);
			UE_LOG(LogTemp, Warning, TEXT("Ship part rotation is: %s"), *loadedShip->partRotation[i].Rotator().ToString());
		}
	}
	else
	{
		//use defualt ship for now
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[0]->Init(1, FVector(0, 0, 700), FQuat(FRotator(0, 0, 0)), 0);
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[1]->Init(0, FVector(0, 0, 0), FQuat(FRotator(0, 0, 0)), 0);
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[2]->Init(0, FVector(-100, 0, 0), FQuat(FRotator(0, 0, 0)), 1);
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[3]->Init(0, FVector(100, 0, 0), FQuat(FRotator(0, 0, 0)), 1);
	}

	

	/*for (int i = 4; i < 204; i++)
	{
		shipPartsInfo.Add(NewObject<UShipPart>());
		shipPartsInfo[i]->Init(0, FVector(0, 0, 0), FQuat(FRotator(0, 0, 0)),0);
	}*/

	/*shipPartsInfo.Add(NewObject<UShipPart>());
	shipPartsInfo[4]->Init(0, FVector(0, -100, 0), FQuat(FRotator(0, 0, 0)));
	shipPartsInfo.Add(NewObject<UShipPart>());
	shipPartsInfo[5]->Init(0, FVector(-100, -100, 0), FQuat(FRotator(0, 0, 0)));
	shipPartsInfo.Add(NewObject<UShipPart>());
	shipPartsInfo[6]->Init(0, FVector(100, -100, 0), FQuat(FRotator(0, 0, 0)));*/

	//shipPartsInfo.Add(NewObject<UShipPart>());
	//shipPartsInfo[7]->Init(0, FVector(0, 100, 0), FQuat(FRotator(0, 0, 0)));
	//shipPartsInfo.Add(NewObject<UShipPart>());
	//shipPartsInfo[8]->Init(0, FVector(-100, 100, 0), FQuat(FRotator(0, 0, 0)));
	//shipPartsInfo.Add(NewObject<UShipPart>());
	//shipPartsInfo[9]->Init(0, FVector(100, 100, 0), FQuat(FRotator(0, 0, 0)));

	BuildShip();
}

// Called every frame
void ABaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timmer += DeltaTime;
	timmerB += DeltaTime;

	if (!moveSelf && !lastMoveSelf)
	{
		SetActorLocation(FVector(0, 0, 0));
	}

	//fix the updates
	if (timmerB > 0.1)
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("The start velocity value is: %s"), *mesh->GetPhysicsLinearVelocity().ToString());
		
		//FVector v = mesh->GetPhysicsLinearVelocity();
		if (throttle != 0.f)
		{
			//thrust going backwards so velocity is forward
			FVector velocity = (FQuat(GetActorRotation()) * (FVector::ForwardVector * -1)) * (throttle * 1000);
			//mesh->SetPhysicsLinearVelocity(velocity, true);
			//mesh->AddForce(FQuat(GetActorRotation())* FVector(-2000000, 0, 0)* throttle);			
			//v += velocity;
			GLog->Log("Amount of Engines!!" + FString::FromInt(shipEngines.Num()));
			for (int i = 0; i < shipEngines.Num();i++)
			{
				if (shipEngines[i])
				{
					FVector force = shipEngines[i]->GetThrust(throttle);
				//	mesh->AddForceAtLocation(force, shipEngines[i]->GetThrustPoint());
					mesh->AddImpulseAtLocation(force, shipEngines[i]->GetThrustPoint());
				//	UE_LOG(LogTemp, Warning, TEXT("The velocity value is: %s"), *mesh->GetPhysicsLinearVelocity().ToString());
				}
			}
		}
		
		//Add Gravity
	/*	if (timmer > 3)
		{
			timmer = 10.f;*/
		if (nearestPlanet)
		{
			FVector gravity = GravityCalculator::CalculateGravityForObject(shipMass * 0.0001, GetActorLocation(), nearestPlanet->planetCentre);
			//mesh->AddForce(gravity);
			mesh->AddImpulse(gravity);
		}
			

		//	UE_LOG(LogTemp, Warning, TEXT("The velocity value is: %s"), *mesh->GetPhysicsLinearVelocity().ToString());
		//}
	

			float p = CalculateAtompsherePressure();
		//Add drag to the ship
		FVector d = DragCalculator::CalculateDragForObject(0.10f, p, mesh->GetPhysicsLinearVelocity() / 100, 5);
		//mesh->AddForce(d);
		mesh->AddImpulse(d);

	//	UE_LOG(LogTemp, Warning, TEXT("The velocity value is: %s"), *mesh->GetPhysicsLinearVelocity().ToString());
		//currentVelocity = mesh->GetPhysicsLinearVelocity();
	//	mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	//	UE_LOG(LogTemp, Warning, TEXT("The velocity value is: %s"), *mesh->GetPhysicsLinearVelocity().ToString());
		//mesh->SetPhysicsLinearVelocity(currentVelocity);
		
	//	UE_LOG(LogTemp, Warning, TEXT("The end velocity value is: %s"), *mesh->GetPhysicsLinearVelocity().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("The drag value is: %s"), *d.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Atmoshere pressure: %f"), p);

		//UE_LOG(LogTemp, Warning, TEXT("The velocity value is: %s"), *mesh->GetPhysicsLinearVelocity().ToString());
		//if (!moveSelf)
		//{
		//	if (solarSystemController)
		//	{
		//		//check if the last move is the same as this move, if not will need to move ship and planet
		//		if (lastMoveSelf != moveSelf) { solarSystemController->RecentreSolarSystem(GetActorLocation()); }
		//		lastMoveSelf = moveSelf;
		//		solarSystemController->MovePlanets((mesh->GetPhysicsLinearVelocity() * -1) * 0.1);
		//	}		
		//}	
		//else
		//{
		//	lastMoveSelf = moveSelf;
		//}
		//mesh->SetPhysicsLinearVelocity(v, false);
		FVector shipPos = GetActorLocation();
		UpdateShipSpeed(shipPos, timmerB);
		UpdateShipAttuiteSeaLevel(shipPos);
		UpdateShipAttuiteTerrainLevel(shipPos);

		//if attuited from terrain is greater than 30km then flip movement method
		if (shipAttuiteTerrainLevel > 30000) 
		{ 
			moveSelf = false; 
		}
		else 
		{
			moveSelf = true; 
		}

		timmerB = 0.f;
	}

	//timer for moving plaent mesh has to be a lot short if not planet can be seen moving
	if (timmer > 0.01)
	{
		if (!moveSelf)
		{
			if (solarSystemController)
			{
				//check if the last move is the same as this move, if not will need to move ship and planet
				if (lastMoveSelf != moveSelf) { solarSystemController->RecentreSolarSystem(GetActorLocation()); }
				lastMoveSelf = moveSelf;
				solarSystemController->MovePlanets((mesh->GetPhysicsLinearVelocity() * -1) * timmer);
			}
		}
		else
		{
			lastMoveSelf = moveSelf;
		}
		timmer = 0.0f;
	}

	if (!moveSelf && !lastMoveSelf)
	{
		SetActorLocation(FVector(0, 0, 0));
	}
		
	//FVector(-7060944.000000, 787018.562500, 7057716.000000)

}

void ABaseShip::ThrottleUp()
{
	if (throttle + 1 <= throttleMax) { throttle += 1; }
	else { throttle = throttleMax; }
}

void ABaseShip::ThrottleDown()
{
	if (throttle - 1 >= 0) { throttle -= 1; }
	else { throttle = 0; }
}

void ABaseShip::PitchUp()
{
	FVector a = (FQuat(GetActorRotation()) * (FVector(0, 1, 0) * turnSensitvity));
	mesh->SetPhysicsAngularVelocityInDegrees(a, true);
}

void ABaseShip::PitchDown()
{
	FVector a = (FQuat(GetActorRotation()) * (FVector(0, -1, 0) * turnSensitvity));
	mesh->SetPhysicsAngularVelocityInDegrees(a, true);
}

void ABaseShip::YawLeft()
{
	FVector a = (FQuat(GetActorRotation()) * (FVector(0, 0, -1) * turnSensitvity));
	mesh->SetPhysicsAngularVelocityInDegrees(a, true);
}

void ABaseShip::YawRight()
{
	FVector a = (FQuat(GetActorRotation()) * (FVector(0, 0, 1) * turnSensitvity));
	mesh->SetPhysicsAngularVelocityInDegrees(a, true);
}

void ABaseShip::RollLeft()
{
	FVector a = (FQuat(GetActorRotation()) * (FVector(-1, 0, 0) * turnSensitvity));
	mesh->SetPhysicsAngularVelocityInDegrees(a, true);
}

void ABaseShip::RollRight()
{
	FVector a = (FQuat(GetActorRotation()) * (FVector(1, 0, 0) * turnSensitvity));
	mesh->SetPhysicsAngularVelocityInDegrees(a, true);
}

void ABaseShip::MoveUp()
{
	FVector velocity = FQuat(GetActorRotation()) * ((FVector::UpVector * 1) * 100);
	mesh->SetPhysicsLinearVelocity(velocity, true);
}

void ABaseShip::MoveDown()
{
	FVector velocity = FQuat(GetActorRotation()) * ((FVector::UpVector * -1) * 100);
	mesh->SetPhysicsLinearVelocity(velocity, true);
}

void ABaseShip::UpdateShipSpeed(FVector newShipPosistion,float time)
{
	float dis = (mesh->GetPhysicsLinearVelocity().Size() * time) / 20;
	//float dis = FVector::Dist(newShipPosistion, lastShipPosistion) / 20;
	shipSpeed = dis / time;
	//lastShipPosistion = newShipPosistion;
	//GLog->Log("Speed : " + FString::FromInt(shipSpeed));
}

void ABaseShip::UpdateShipAttuiteSeaLevel(FVector newShipPosistion)
{
	if (!nearestPlanet) 
	{ 
		GLog->Log("No vaild Planet");
		return; 
	}
	float dis = FVector::Dist(nearestPlanet->planetCentre, newShipPosistion);
	shipAttuiteSeaLevel = (dis - nearestPlanet->planetRadius) / 20;
//	GLog->Log("ship Attuite Sea Level : " + FString::FromInt(shipAttuiteSeaLevel));
}

void ABaseShip::UpdateShipAttuiteTerrainLevel(FVector newShipPosistion)
{
	if (!nearestPlanet)
	{
		GLog->Log("No vaild Planet");
		return;
	}
	FHitResult results;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	//
	bool hit = GetWorld()->LineTraceSingleByChannel(results, newShipPosistion, nearestPlanet->planetCentre, 
		ECollisionChannel::ECC_GameTraceChannel1/*, CollisionParams*/);
	if (hit)
	{
		shipAttuiteTerrainLevel = FVector::Dist(results.Location, newShipPosistion) / 20;
	}
//s	GLog->Log("ship Attuite Terrain Level : " + FString::FromInt(shipAttuiteTerrainLevel));
}

float ABaseShip::CalculateAtompsherePressure()
{
	float surfaceAP = 1.21f;
	float atmosphereDis = 40000;
	float pointInAtmosphere = atmosphereDis - shipAttuiteSeaLevel;
	if (pointInAtmosphere < 0) { return 0.f; }
	return (surfaceAP / atmosphereDis) * pointInAtmosphere;
}

void ABaseShip::BuildShip()
{
	if(!cockpitStaticMeshes){ cockpitStaticMeshes = GetWorld()->SpawnActor<ACockpitStaticMehses>(FVector(0, 0, 0), FRotator(0, 0, 0)); }
	int mainCockPitIndex = 0;
	for (int i = 0; i < shipPartsInfo.Num();i++)
	{
		if (shipPartsInfo[i]->partID == 1)
		{
			ABaseShipPart* newPart = GetWorld()->SpawnActor<ABaseShipPart>
				(PartsList::GetShipPart(shipPartsInfo[i]->partID), GetActorLocation(), GetActorRotation());
			if (newPart)
			{
				mainCockPitIndex = i;
				//set mesh properties to the ship part
				mesh->SetStaticMesh(newPart->mesh->GetStaticMesh());
				mesh->SetMassOverrideInKg("Mesh", newPart->partWeight);
				shipMass += newPart->partWeight;
				shipCentreOfMass = newPart->partCentre;
				//set to -1 as its the root
				attachmentInfo.Add(shipPartsInfo[i]->attachedTo, -1);
				newPart->Destroy();
			}
			//mesh->SetStaticMesh(cockpitStaticMeshes->cockpitMeshes.FindRef(shipPartsInfo[i]->partID)->GetStaticMesh());		
		}
	}
	for (int i = 0; i < shipPartsInfo.Num();i++)	
	{
		if (shipPartsInfo[i]->partID == 1 && !mainCockPitIndex)
		{
			//ABaseShipPart* newPart = GetWorld()->SpawnActor<ABaseShipPart>
			//	(PartsList::GetShipPart(shipPartsInfo[i]->partID), GetActorLocation(), GetActorRotation());
			//if (newPart)
			//{
			//	//set mesh properties to the ship part
			//	mesh->SetStaticMesh(newPart->mesh->GetStaticMesh());
			//	mesh->SetMassOverrideInKg("Mesh", newPart->partWeight);
			//	shipMass += newPart->partWeight;
			//	shipCentreOfMass = newPart->partCentre;
			//	//set to -1 as its the root
			//	attachmentInfo.Add(shipPartsInfo[i]->attachedTo, -1);
			//	newPart->Destroy();
			//}
			//mesh->SetStaticMesh(cockpitStaticMeshes->cockpitMeshes.FindRef(shipPartsInfo[i]->partID)->GetStaticMesh());		
		}
		else
		{
			ABaseShipPart* newPart = GetWorld()->SpawnActor<ABaseShipPart>
				(PartsList::GetShipPart(shipPartsInfo[i]->partID), GetActorLocation(), GetActorRotation());
			if (newPart)
			{
				FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, true);

				newPart->AttachToComponent(RootComponent, attachRules);

				newPart->SetActorRelativeLocation(shipPartsInfo[i]->partPosition - shipPartsInfo[mainCockPitIndex]->partPosition);
				newPart->SetActorRelativeRotation(shipPartsInfo[i]->partRotation - shipPartsInfo[mainCockPitIndex]->partRotation);
				//newPart->attachment = shipPartsInfo[i]->attachedTo;
				shipMass += newPart->partWeight;				
				int index = shipParts.Add(newPart);
				attachmentInfo.Add(shipPartsInfo[i]->attachedTo, index);
				//spilt ship into seprate arrays
				SpiltShip(newPart);
			}
		}	
	}
	//set up main attachments
	//for (int i = 0; i < shipPartsInfo.Num();i++)
	//{
	//	int attachPoint = attachmentInfo.FindRef(shipPartsInfo[i]->attachedTo);
	//	//check if attach point is root componet (-1)
	//	if (attachPoint != -1)
	//	{
	//		//shipParts[attachPoint]->
	//	}
	//}

	mesh->SetCenterOfMass(CalculateIntialCentreOfMass());
	TurnOnCollisionsAndPhysics();
}

void ABaseShip::SpiltShip(ABaseShipPart* part)
{
	//check if part is an engine
	ABaseShipPartEngine* engine = Cast<ABaseShipPartEngine>(part);
	if (engine) { shipEngines.Add(engine); }
}

void ABaseShip::TurnOnCollisionsAndPhysics()
{
	for (int i = 0; i < shipParts.Num();i++)
	{
		shipParts[i]->ToggleCollision(true);
	}
}

FVector ABaseShip::CalculateIntialCentreOfMass()
{
	TMap<float, float> xv;
	TMap<float, float> yv;
	TMap<float, float> zv;
	//cockpit won't be in ship parts so add that to start with
	FVector centreOfMass = FVector(0, 0, 0);//MassPosition(shipCentreOfMass, mesh->GetMass());
	xv.Add(shipCentreOfMass.X, mesh->GetMass());
	yv.Add(shipCentreOfMass.Y, mesh->GetMass());
	zv.Add(shipCentreOfMass.Z, mesh->GetMass());

	for (int i = 0; i < shipParts.Num();i++)
	{	
		//work out the point
		FQuat t = FQuat(GetActorRotation());
		FVector point = t.UnrotateVector((shipParts[i]->GetActorLocation()/* + shipParts[i]->partCentre*/) - GetActorLocation());
		point +=  shipParts[i]->partCentre;
		point = FVector(FMath::RoundHalfFromZero(point.X), FMath::RoundHalfFromZero(point.Y), FMath::RoundHalfFromZero(point.Z));
		//check if vaules are the same if not add them to array
		if (!xv.Contains(point.X)) { xv.Add(point.X, shipParts[i]->partWeight); }
		else { float m = 0.f; xv.RemoveAndCopyValue(point.X, m); xv.Add(point.X, m + shipParts[i]->partWeight); }
		if (!yv.Contains(point.Y)) { yv.Add(point.Y, shipParts[i]->partWeight); }
		else { float m = 0.f; yv.RemoveAndCopyValue(point.Y, m); yv.Add(point.Y, m + shipParts[i]->partWeight); }
		if (!zv.Contains(point.Z)) { zv.Add(point.Z, shipParts[i]->partWeight); }
		else { float m = 0.f; zv.RemoveAndCopyValue(point.Z, m); zv.Add(point.Z, m + shipParts[i]->partWeight); }
	}

	centreOfMass = FVector(MassPositionForArray(&xv), MassPositionForArray(&yv), MassPositionForArray(&zv));
	UE_LOG(LogTemp, Warning, TEXT("Current Centre of mass = : %s"), *centreOfMass.ToString());
	centreOfMass = centreOfMass / shipMass;
	
	shipCentreOfMass = centreOfMass;
	UE_LOG(LogTemp, Warning, TEXT("Final Centre of mass = : %s"), *centreOfMass.ToString());
	return shipCentreOfMass;
}

FVector ABaseShip::MassPosition(FVector RelativePosition, float mass)
{
	return FVector(mass * RelativePosition.X, mass * RelativePosition.Y, mass * RelativePosition.Z);
}

float ABaseShip::MassPositionForArray(TMap<float, float>* points)
{
	float rf = 0.f;
	TArray<float> massPoints;
	TArray<float> masses;
	points->GenerateKeyArray(massPoints);
	points->GenerateValueArray(masses);
	for (int i = 0; i < massPoints.Num();i++)
	{
		rf += masses[i] * massPoints[i];
	}
	return rf;
}



