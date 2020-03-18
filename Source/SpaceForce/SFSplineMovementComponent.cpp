// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSplineMovementComponent.h"
#include "Components/SplineComponent.h"
#include "SFFlightPath.h"

// Sets default values for this component's properties
USFSplineMovementComponent::USFSplineMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USFSplineMovementComponent::SetFlightPath(ASFFlightPath* FP, float distance)
{
	FlightPath = FP;
	distanceAlongPath = distance;

}

FVector USFSplineMovementComponent::GetLocationAtDistanceAlongSpline(float distance, bool bWithOffset)
{
	if (FlightPath == NULL)
	{
		return FVector();
	}
	
	FVector Location = FlightPath->Spline->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);

	if (bWithOffset) {
		FVector CurrentLocation = FlightPath->Spline->GetLocationAtDistanceAlongSpline(distanceAlongPath, ESplineCoordinateSpace::World);
		USceneComponent* Root = GetOwner()->GetRootComponent();
		FVector OffsetFromPath = Root->GetComponentLocation() - CurrentLocation;
		Location += OffsetFromPath;
	}
	return Location;
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

	float nextDistance = DeltaTime * speed + distanceAlongPath;

	FVector LastLocation = FlightPath->Spline->GetLocationAtDistanceAlongSpline(distanceAlongPath, ESplineCoordinateSpace::World);
	FVector NextLocation = FlightPath->Spline->GetLocationAtDistanceAlongSpline(nextDistance, ESplineCoordinateSpace::World);
	
	Root->AddWorldOffset(NextLocation - LastLocation, true, NULL, ETeleportType::None);

	if (bOrientToSpline)
	{
		FQuat NextRotation = FlightPath->Spline->GetQuaternionAtDistanceAlongSpline(nextDistance, ESplineCoordinateSpace::World);
		Root->SetWorldRotation(NextRotation, true, NULL, ETeleportType::None);
	}

	distanceAlongPath = nextDistance;
	if (distanceAlongPath >= FlightPath->Spline->GetSplineLength() && NextFlightPath != NULL)
	{
		SetFlightPath(NextFlightPath, 0.0f);
		NextFlightPath = NULL;
	}
}

