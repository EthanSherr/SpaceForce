// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSplineMovementComponent.h"
#include "Components/SplineComponent.h"
#include "SFFlightPath.h"

// Sets default values for this component's properties
USFSplineMovementComponent::USFSplineMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	//SetTickGroup(ETickingGroup::TG_PrePhysics);
}

void USFSplineMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	UE_LOG(LogTemp, Warning, TEXT("InitializeComponent"))
}

void USFSplineMovementComponent::UpdateTickRegistration()
{
	Super::UpdateTickRegistration();
	UE_LOG(LogTemp, Warning, TEXT("UpdateTickRegistration"))
}

void USFSplineMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("UpdatedComponent %s"), *UpdatedComponent->GetName())
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

void USFSplineMovementComponent::SetNextFlightPath(ASFFlightPath* FP)
{
	NextFlightPath = FP;
}

ASFFlightPath* USFSplineMovementComponent::GetFlightPath()
{
	return FlightPath;
}

void USFSplineMovementComponent::SetSpeed(float s)
{
	speed = s;
}

float USFSplineMovementComponent::GetSpeed()
{
	return speed;
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
		return;
	}

	float deltaDistance = DeltaTime * speed;
	float nextDistance = deltaDistance + distanceAlongPath;

	FVector NextLocation = FlightPath->GetLocationAtDistance(nextDistance);
	FVector DeltaLocation = NextLocation - LastLocation;

	if (bVerboseDelta) {
		UE_LOG(LogTemp, Warning, TEXT("d %f \t\tnextDistance %f"),
			deltaDistance - (DeltaLocation).Size(),
			nextDistance)
	}

	FVector v = UpdatedComponent->GetComponentVelocity();

	//UE_LOG(LogTemp, Warning, TEXT("1 UpdatedComopnent is %s Component.GetComponentVelocity (%s)"), 
	//	*UpdatedComponent->GetName(),
	//	*UpdatedComponent->GetComponentVelocity().ToString())
	//UE_LOG(LogTemp, Warning, TEXT("2 Owner is (%s) and its root is (%s) Actor.GetVelocity (%s)"), 
	//	*GetOwner()->GetName(), 
	//	*GetOwner()->GetRootComponent()->GetName(), 
	//	*GetOwner()->GetVelocity().ToString())

	FQuat NextRotation;
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

