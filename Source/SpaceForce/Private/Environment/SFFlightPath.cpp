// Fill out your copyright notice in the Description page of Project Settings.


#include "SFFlightPath.h"
#include "SpaceForce.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

ASFFlightPath::ASFFlightPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	Spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("SplinePath"));
	SplineStepSize = 2.0f;
	bPathForPlayer = false;
}

void ASFFlightPath::OnConstruction(const FTransform& Transform)
{
	if (bPathForPlayer) {
		SetupStartSphere();
		Spline->EditorUnselectedSplineSegmentColor = FColor::Blue;
		Spline->bShouldVisualizeScale = true;
		Spline->ScaleVisualizationWidth = 10.0f;
	}

	Points.Empty();
	if (bUseCustomInterpolation) {
		SetupSpline();
	}
}

void ASFFlightPath::SetupStartSphere()
{
	FVector BeginSphereLocation = Spline->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
	BeginSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
	BeginSphere->SetSphereRadius(30.f);
	BeginSphere->RegisterComponentWithWorld(GetWorld());
	BeginSphere->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	BeginSphere->SetWorldLocation(BeginSphereLocation, false, NULL, ETeleportType::TeleportPhysics);
	BeginSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	BeginSphere->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Ignore);
}

void ASFFlightPath::SetupSpline()
{
	if (SplineStepSize <= 0.0f) {
		UE_LOG(LogTemp, Error, TEXT("SplineSmoothingStepSize must be more than 0, defaulting to 2.0"))
		SplineStepSize = 2.0f;
	}

	int Segments = ceil(Spline->GetSplineLength() / SplineStepSize);
	FSplineDistance SDLast;
	Points.SetNum(Segments - 1);
	for (int i = 0; i < Segments; i++) {
		FSplineDistance SD = FSplineDistance();
		SD.Location = Spline->GetLocationAtDistanceAlongSpline(i * SplineStepSize, ESplineCoordinateSpace::World);
		if (i != 0)
		{
			FVector Delta = SD.Location - SDLast.Location;
			SD.Distance = Delta.Size() + SDLast.Distance;
			SDLast.Forward = Delta.GetSafeNormal();
			Points[i - 1] = SDLast;
		}
		SDLast = SD;
	}

	UE_LOG(LogTemp, Warning, TEXT("Points.last %s, size %d seg %d"), *Points.Last().ToString(), Points.Num(), Segments)
}

FVector ASFFlightPath::GetLocationAtDistance(float distance)
{
	if (bUseCustomInterpolation) {
		FSplineDistance start, end;
		if (PointsForDistance(distance, start, end)) {
			float a = (distance - start.Distance) / (end.Distance - start.Distance);
			return a * start.Forward * (end.Distance - start.Distance) + start.Location;
		}
		return FVector::ZeroVector;
	}
	else {
		return Spline->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	}
}

bool ASFFlightPath::PointsForDistance(float Distance, FSplineDistance& OutStart, FSplineDistance& OutEnd) {
	if (Points.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("No points are computed, cannot return PointsForDistance"))
		return false;
	}

	if (Distance < 0.0f || Distance >= Points.Last().Distance) {
		UE_LOG(LogTemp, Warning, TEXT("distance %f exceeds [0, %f]"), Distance, Points.Last().Distance)
		return false;
	}

	int distanceIndex = Distance / SplineStepSize;
	int iterations = 0;
	while (distanceIndex >= 0 && distanceIndex + 1 <= Points.Num()) {
		if ((Points[distanceIndex].Distance <= Distance && Distance <= Points[distanceIndex + 1].Distance) || iterations > 80000) {
			break;
		}

		if (Points[distanceIndex].Distance > Distance) {
			distanceIndex--;
		} else
		if (Distance > Points[distanceIndex + 1].Distance) {
			distanceIndex++;
		}
		iterations++;
	}
	if (iterations > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Iterations %d"), iterations)
	}
	if (distanceIndex + 1 >= Points.Num()) {
		return false;
	}

	OutStart = Points[distanceIndex];
	OutEnd = Points[distanceIndex + 1];

	DrawDebugPoint(GetWorld(), OutStart.Location, 25, FColor::Blue, false, 1);
	DrawDebugPoint(GetWorld(), OutEnd.Location, 25, FColor::Red, false, 1);
	return true;
}

//Closest point on spline to location begin

FVector ASFFlightPath::FindLocationClosestToWorldLocation(FVector Target) {
	if (!Spline) {
		return FVector::ZeroVector;
	}
	return Spline->FindLocationClosestToWorldLocation(Target, ESplineCoordinateSpace::World);
}

//Closest point on spline to location end