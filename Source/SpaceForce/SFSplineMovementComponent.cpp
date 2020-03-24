// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSplineMovementComponent.h"
#include "Components/SplineComponent.h"
#include "SFFlightPath.h"

// Sets default values for this component's properties
USFSplineMovementComponent::USFSplineMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetTickGroup(ETickingGroup::TG_PrePhysics);
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
		USceneComponent* Root = GetOwner()->GetRootComponent();
		FVector OffsetFromPath = Root->GetComponentLocation() - CurrentLocation;
		Location += OffsetFromPath;
	}
	return Location;
}

void USFSplineMovementComponent::InitializeComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("InitializeComponent"))
}

void USFSplineMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	SetFlightPath(FlightPath, initialOffset);
}

// Called every frame
void USFSplineMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	USceneComponent* Root = GetOwner()->GetRootComponent();
	if (Root == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot move component, root is null"))
		return;
	}

	if (FlightPath == NULL)
	{
		return;
	}

	float deltaDistance = DeltaTime * speed;
	float nextDistance = deltaDistance + distanceAlongPath;

	FVector NextLocation = FlightPath->GetLocationAtDistance(nextDistance);

	UE_LOG(LogTemp, Warning, TEXT("d %f \t\tnextDistance %f"),
		deltaDistance - (NextLocation - LastLocation).Size(),
		nextDistance)

	Root->AddWorldOffset(NextLocation - LastLocation, true, NULL, ETeleportType::None);

	if (bOrientToSpline)
	{
		FQuat NextRotation = FlightPath->Spline->GetQuaternionAtDistanceAlongSpline(nextDistance, ESplineCoordinateSpace::World);
		Root->SetWorldRotation(NextRotation, true, NULL, ETeleportType::None);
	}

	distanceAlongPath = nextDistance;
	LastLocation = NextLocation;

	if (distanceAlongPath >= FlightPath->Spline->GetSplineLength() && NextFlightPath != NULL)
	{
		SetFlightPath(NextFlightPath, 0.0f);
		NextFlightPath = NULL;
	}
}

