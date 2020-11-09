// Fill out your copyright notice in the Description page of Project Settings.


#include "SFFlyingAIPawn.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/SFSpringFlightMovementComponent.h"
#include "../Components/SFHealthComponent.h"

ASFFlyingAIPawn::ASFFlyingAIPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	ShipStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("ShipStaticMesh"));
	ShipStaticMesh->SetSimulatePhysics(true);
	ShipStaticMesh->SetEnableGravity(false);

	RootComponent = ShipStaticMesh;

	FlightMovement = ObjectInitializer.CreateDefaultSubobject<USFSpringFlightMovementComponent>(this, FName("FlightMovement"));
	FlightMovement->LinearMaxSpeed = 0.0f;
	FlightMovement->AngularStiffnessPrimary = 115.0f;
	FlightMovement->AngularStiffnessSecondary = 275.0f;

	HealthComponent = ObjectInitializer.CreateDefaultSubobject<USFHealthComponent>(this, FName("HealthComponent"));
	HealthComponent->Health = 100.0f;
}