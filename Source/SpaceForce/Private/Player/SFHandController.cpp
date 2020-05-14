// Fill out your copyright notice in the Description page of Project Settings.


#include "SFHandController.h"
#include "Components/SphereComponent.h"
#include "../Environment/SFFlightPath.h"

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

	PathScanner = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("PathScanner"));
	PathScanner->SetSphereRadius(1000.0f);
	PathScanner->bHiddenInGame = false;
	PathScanner->SetupAttachment(this);
	PathScanner->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PathScanner->SetCollisionResponseToAllChannels(ECR_Ignore);
}

ASFFlightPath* USFHandController::GetNearestFlightPath() {
	TArray<AActor*> Overlapping;
	PathScanner->GetOverlappingActors(Overlapping, ASFFlightPath::StaticClass());
	float MinDistance;
	ASFFlightPath* NearestFlightPath = NULL;
	for (AActor* Actor : Overlapping) {
		ASFFlightPath* FlightPath = Cast<ASFFlightPath>(Actor);
		float NewDistance = FVector::Distance(this->GetComponentLocation(), FlightPath->GetActorLocation());
		if (!NearestFlightPath || NewDistance < MinDistance) {
			MinDistance = NewDistance;
			NearestFlightPath = FlightPath;
		}
	}
	return NearestFlightPath;
}

void USFHandController::BeginDriving(AActor* Actor) {
	HandState = EHandState::Driving;
}