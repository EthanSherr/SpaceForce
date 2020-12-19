#include "Player/SFPlayerShip.h"
#include "Player/SFPilotPawn.h"
#include "Components/SFBoosterManagerComponent.h"
#include "Components/SFSplineMovementComponent.h"

ASFPlayerShip::ASFPlayerShip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	BoosterManagerComponent = ObjectInitializer.CreateDefaultSubobject<USFBoosterManagerComponent>(this, FName("BoosterManagerComponent"));
}

void ASFPlayerShip::TrySetIsBoosting(bool bNewIsBoosting)
{
	BoosterManagerComponent->TrySetIsBoosting(bNewIsBoosting);
}

ASFPilotPawn* ASFPlayerShip::GetOwnerPilot() const
{
	return Cast<ASFPilotPawn>(GetOwner());
}

USFSplineMovementComponent* ASFPlayerShip::GetAssociatedSplineMovementComponent() {
	//if (AssociatedSplineMovementComponent) {
	//	return AssociatedSplineMovementComponent;
	//}
	auto Pilot = GetOwnerPilot();
	if (Pilot) 
	{
		return Pilot->SplineMovement;
	}
	return NULL;
	//auto TargetComponent = FlightMovement->TargetComponent;
	//if (TargetComponent) {
	//	auto TargetOwner = TargetComponent->GetOwner();
	//	AssociatedSplineMovementComponent = Cast<USFSplineMovementComponent>(TargetOwner->GetComponentByClass(USFSplineMovementComponent::StaticClass()));
	//	if (AssociatedSplineMovementComponent) {
	//		return AssociatedSplineMovementComponent;
	//	}
	//}
	//AssociatedSplineMovementComponent = Cast<USFSplineMovementComponent>(GetComponentByClass(USFSplineMovementComponent::StaticClass()));
	//return AssociatedSplineMovementComponent;
}
