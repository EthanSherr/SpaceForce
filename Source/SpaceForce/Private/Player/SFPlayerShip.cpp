#include "Player/SFPlayerShip.h"
#include "Player/SFPilotPawn.h"
#include "Player/SFHandController.h"
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
	auto* Pilot = GetOwnerPilot();
	if (Pilot) 
	{
		return Pilot->SplineMovement;
	}
	return NULL;
}