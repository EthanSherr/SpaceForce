// Fill out your copyright notice in the Description page of Project Settings.


#include "SFShipPawn.h"
#include "Components/StaticMeshComponent.h"
#include "../Ship/SFSpringFlightMovementComponent.h"
#include "../Components/SFSplineMovementComponent.h"
#include "../Environment/SFFlightPath.h"
#include "SFPilotPawn.h"
#include "SpaceForce.h"

ASFShipPawn::ASFShipPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	ShipStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("ShipStaticMesh"));
	ShipStaticMesh->SetSimulatePhysics(true);
	ShipStaticMesh->SetCollisionProfileName(COLLISION_PROFILE_PAWN);
	RootComponent = ShipStaticMesh;

	FlightMovement = ObjectInitializer.CreateDefaultSubobject<USFSpringFlightMovementComponent>(this, FName("FlightMovement"));
	FlightMovement->LinearMaxSpeed = 0.0f;
	FlightMovement->AngularStiffness = 70.0f;
}

ASFPilotPawn* ASFShipPawn::GetOwnerPilot() {
	return Cast<ASFPilotPawn>(GetOwner());
}

USFSplineMovementComponent* ASFShipPawn::GetAssociatedSplineMovementComponent() {
	if (AssociatedSplineMovementComponent) {
		return AssociatedSplineMovementComponent;
	}
	auto Pilot = GetOwnerPilot();
	if (Pilot) {
		AssociatedSplineMovementComponent = Pilot->SplineMovement;
		return AssociatedSplineMovementComponent;
	}
	AssociatedSplineMovementComponent = Cast<USFSplineMovementComponent>(GetComponentByClass(USFSplineMovementComponent::StaticClass()));
	return AssociatedSplineMovementComponent;
}
