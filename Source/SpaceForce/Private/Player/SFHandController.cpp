// Fill out your copyright notice in the Description page of Project Settings.


#include "SFHandController.h"
#include "Components/SphereComponent.h"
#include "../Ship/SFSpringFlightMovementComponent.h"
#include "../Environment/SFFlightPath.h"
#include "SFPilotPawn.h"
#include "SFShipPawn.h"
#include "SpaceForce.h"

USFHandController::USFHandController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	this->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->SetGenerateOverlapEvents(false);

	ShipScanner = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("ShipSweeper"));
	ShipScanner->SetSphereRadius(16.0f);
	ShipScanner->bHiddenInGame = false;
	ShipScanner->SetupAttachment(this);
	ShipScanner->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipScanner->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShipScanner->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ShipScanner->SetCollisionObjectType(ECC_Pawn);

	PathScanner = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("PathScanner"));
	PathScanner->SetSphereRadius(1000.0f);
	PathScanner->SetupAttachment(this);
	PathScanner->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PathScanner->SetCollisionResponseToAllChannels(ECR_Overlap);
	PathScanner->SetCollisionObjectType(ECC_Pawn);
}

ASFFlightPath* USFHandController::GetNearestFlightPath(ASFFlightPath* PathToIgnore) {
	TArray<AActor*> Overlapping;
	PathScanner->GetOverlappingActors(Overlapping, ASFFlightPath::StaticClass());
	float MinDistance = 0.0f;
	ASFFlightPath* NearestFlightPath = NULL;
	for (AActor* Actor : Overlapping) {
		if (PathToIgnore != Actor) {
			ASFFlightPath* FlightPath = Cast<ASFFlightPath>(Actor);
			float NewDistance = FVector::Distance(this->GetComponentLocation(), FlightPath->GetActorLocation());
			if (!NearestFlightPath || NewDistance < MinDistance) {
				MinDistance = NewDistance;
				NearestFlightPath = FlightPath;
			}
		}
	}
	return NearestFlightPath;
}

ASFPilotPawn* USFHandController::GetPilot() {
	return Cast<ASFPilotPawn>(GetOwner());
}

ASFShipPawn* USFHandController::GetShip() {
	auto Pilot = GetPilot();
	if (!Pilot) {
		return NULL;
	}
	return Cast<ASFShipPawn>(Pilot->Ship);
}

ASFShipPawn* USFHandController::GetOverlappingShip() {
	TArray<AActor*> Overlapping;
	ShipScanner->GetOverlappingActors(Overlapping, ASFShipPawn::StaticClass());
	return Overlapping.Num() > 0 ? Cast<ASFShipPawn>(Overlapping[0]) : NULL;
}

TEnumAsByte<EHandState> USFHandController::GetHandState() {
	return HandState;
}

void USFHandController::SetHandState(TEnumAsByte<EHandState> NewState) {
	HandState = NewState;
	PathScanner->SetCollisionEnabled(HandState == EHandState::Driving ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}