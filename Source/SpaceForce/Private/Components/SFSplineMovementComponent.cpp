// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSplineMovementComponent.h"
#include "Components/SplineComponent.h"
#include "SFFlightPath.h"

// Sets default values for this component's properties
USFSplineMovementComponent::USFSplineMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USFSplineMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//UE_LOG(LogTemp, Warning, TEXT("InitializeComponent"))
}

void USFSplineMovementComponent::UpdateTickRegistration()
{
	Super::UpdateTickRegistration();
	//UE_LOG(LogTemp, Warning, TEXT("UpdateTickRegistration"))
}

void USFSplineMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay %s"), *UpdatedComponent->GetName())
	SetFlightPath(FlightPath, initialOffset);
}

void USFSplineMovementComponent::SetFlightPath(ASFFlightPath* FP, float distance)
{
	FlightPath = FP;
	if (FP == NULL) {
		return;
	}
	distanceAlongPath = distance;
	LastLocation = FlightPath->GetLocationAtDistance(distanceAlongPath);
}

ASFFlightPath* USFSplineMovementComponent::GetFlightPath()
{
	return FlightPath;
}

FVector USFSplineMovementComponent::GetLocationAtDistanceAlongSpline(float distance, bool bWithOffset)
{
	if (FlightPath == NULL)
	{
		return FVector();
	}
	
	FVector Location = FlightPath->GetLocationAtDistance(distance);

	if (bWithOffset) {
		FVector CurrentLocation = FlightPath->GetLocationAtDistance(distanceAlongPath);
		FVector OffsetFromPath = UpdatedComponent->GetComponentLocation() - CurrentLocation;
		Location += OffsetFromPath;
	}
	return Location;
}

FVector USFSplineMovementComponent::GetDirectionAtDistanceAlongSpline(float distance)
{
	if (!FlightPath) {
		return FVector::ZeroVector;
	}
	return FlightPath->Spline->GetDirectionAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
}

// Called every frame
void USFSplineMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!UpdatedComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot move component, root is null"))
		return;
	}

	if (!FlightPath)
	{
		if (NextFlightPath) {
			SetFlightPath(NextFlightPath, 0.0f);
			NextFlightPath = NULL;
		}
		return;
	}

	float deltaDistance = DeltaTime * Speed;
	float nextDistance = deltaDistance + distanceAlongPath;

	FVector NextLocation = FlightPath->GetLocationAtDistance(nextDistance);
	FVector DeltaLocation = NextLocation - LastLocation;

	if (bVerboseDelta) {
		UE_LOG(LogTemp, Warning, TEXT("d %f \t\tnextDistance %f"),
			deltaDistance - (DeltaLocation).Size(),
			nextDistance)
	}

	FQuat NextRotation = UpdatedComponent->GetComponentQuat();
	if (bOrientToSpline)
	{
		NextRotation = FlightPath->Spline->GetQuaternionAtDistanceAlongSpline(nextDistance, ESplineCoordinateSpace::World);
	}

	MoveUpdatedComponent(DeltaLocation, NextRotation, true, NULL, ETeleportType::None);

	distanceAlongPath = nextDistance;
	LastLocation = NextLocation;
	Velocity = DeltaLocation / DeltaTime;
	UpdateComponentVelocity();

	// Use the next flight path if current path is over
	if (distanceAlongPath >= FlightPath->Spline->GetSplineLength() && NextFlightPath != NULL)
	{
		SetFlightPath(NextFlightPath, 0.0f);
		NextFlightPath = NULL;
	}
}

