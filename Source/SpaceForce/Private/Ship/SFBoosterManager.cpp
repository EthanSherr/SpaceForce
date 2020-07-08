#include "SFBoosterManager.h"
#include "Components/TimelineComponent.h"
#include "../Player/SFShipPawn.h"
#include "../Player/SFPilotPawn.h"
#include "../Components/SFSplineMovementComponent.h"
#include "../Ship/SFSpringFlightMovementComponent.h"

USFBoosterManager::USFBoosterManager(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	BoostTimeline = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("BoostTimeline"));
	SpeedBoostTimelineUpdateDelegate.BindDynamic(this, &USFBoosterManager::SpeedBoostTimelineUpdate);
	LinearStiffnessBoostTimelineUpdateDelegate.BindDynamic(this, &USFBoosterManager::LinearStiffnessBoostTimelineUpdate);
	BoostTimelineFinishedDelegate.BindDynamic(this, &USFBoosterManager::BoostTimelineFinished);

	SpeedBoostDecay = 200.0f;
	LinearStiffnessBoostDecay = 50.0f;

	MaximumBoosterEnergy = 100.0f;
	MinimumEnergyToStartBoost = 25.0f;
	BoostEnergyDecayRate = 25.0f;
	BoostEnergyRegenRate = 5.0f;
}

void USFBoosterManager::BeginPlay()
{
	Super::BeginPlay();

	if (SpeedBoostCurve)
	{
		BoostTimeline->AddInterpFloat(SpeedBoostCurve, SpeedBoostTimelineUpdateDelegate, FName("Speed"));
		BoostTimeline->SetTimelineFinishedFunc(BoostTimelineFinishedDelegate);
	}

	if (LinearStiffnessBoostCurve)
		BoostTimeline->AddInterpFloat(LinearStiffnessBoostCurve, LinearStiffnessBoostTimelineUpdateDelegate, FName("Handling"));

	BoosterEnergy = MaximumBoosterEnergy;
}

void USFBoosterManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Boost Speed / Handling Decay
	if (!bIsBoosting)
	{
		if (SpeedBoostSpeedDelta != 0.0)
		{
			float NextBoostTimelineSpeedDelta = SpeedBoostSpeedDelta - SpeedBoostDecay * DeltaTime;
			if (NextBoostTimelineSpeedDelta < 0.1)
				NextBoostTimelineSpeedDelta = 0.0f;

			SetSpeedBoostDelta(NextBoostTimelineSpeedDelta);
		}
		if (LinearStiffnessBoostDelta)
		{
			float NextBoostTimelineHandlingDelta = LinearStiffnessBoostDelta - LinearStiffnessBoostDecay * DeltaTime;
			if (NextBoostTimelineHandlingDelta < 0.1)
				NextBoostTimelineHandlingDelta = 0.0f;

			SetLinearStiffnessBoostDelta(NextBoostTimelineHandlingDelta);
		}
	}

	const float BoosterEnergyDelta = (bIsBoosting ? -BoostEnergyDecayRate : BoostEnergyRegenRate) * DeltaTime;
	const float LastBoosterEnergy = BoosterEnergy;
	BoosterEnergy = FMath::Clamp(BoosterEnergyDelta + BoosterEnergy, 0.0f, MaximumBoosterEnergy);

	if (LastBoosterEnergy != BoosterEnergy)
		OnBoosterEnergyChanged.Broadcast(BoosterEnergy, MaximumBoosterEnergy, LastBoosterEnergy);

	if (bIsBoosting && BoosterEnergy == 0.0f)
	{
		TrySetIsBoosting(false);
	}
}

void USFBoosterManager::TrySetIsBoosting(bool bNewIsBoosting)
{
	bool bSuccess = true;
	if (bNewIsBoosting)
	{
		bSuccess = BoosterEnergy > MinimumEnergyToStartBoost;
		if (bSuccess)
			BoostTimeline->PlayFromStart();
	}
	else
	{
		BoostTimeline->Stop();
	}

	if (bSuccess)
		bIsBoosting = bNewIsBoosting;
}

void USFBoosterManager::SetSpeedBoostDelta(float NewBoostSpeed)
{
	auto Ship = Cast<ASFShipPawn>(GetOwner());
	if (!Ship || !Ship->GetOwnerPilot())
		return;

	auto SplineMovement = Ship->GetOwnerPilot()->SplineMovement;
	SplineMovement->Speed = SplineMovement->Speed - SpeedBoostSpeedDelta + NewBoostSpeed;
	SpeedBoostSpeedDelta = NewBoostSpeed;
}

void USFBoosterManager::SetLinearStiffnessBoostDelta(float NewBoostHandling)
{
	auto Ship = Cast<ASFShipPawn>(GetOwner());
	if (!Ship)
		return;

	const float NewLinearStiffness = Ship->FlightMovement->LinearStiffness - LinearStiffnessBoostDelta + NewBoostHandling;
	Ship->FlightMovement->SetLinearStiffness(NewLinearStiffness);
	LinearStiffnessBoostDelta = NewBoostHandling;
}

void USFBoosterManager::SpeedBoostTimelineUpdate(float Value)
{
	SetSpeedBoostDelta(Value);
}

void USFBoosterManager::LinearStiffnessBoostTimelineUpdate(float Value)
{
	SetLinearStiffnessBoostDelta(Value);
}

void USFBoosterManager::BoostTimelineFinished()
{
	if (BoostTimeline->GetPlaybackPosition() == 0.0f)
	{

	}
}