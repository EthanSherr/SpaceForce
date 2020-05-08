// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineHelpers.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"

FVector USplineHelpers::ClosestPointOnSplineToLocation(USplineComponent* SplineComponent, FVector Target, float Min, float Max, int MaxIterations, bool bDebug) {
	if (!SplineComponent) {
		return FVector::ZeroVector;
	}
	if (Max < 0) {
		Max = SplineComponent->GetSplineLength();
	}

	float LengthAlongSpline = Min;
	FVector LocationOnSpline;
	float TargetDelta;
	SplineData(SplineComponent, Target, LengthAlongSpline, LocationOnSpline, TargetDelta);

	for (int i = 0; i < MaxIterations; i++) {
		float NextLengthAlongSpline = (Max - Min) / 2 + Min;
		FVector NextLocationOnSpline;
		float NextTargetDelta;
		SplineData(SplineComponent, Target, NextLengthAlongSpline, NextLocationOnSpline, NextTargetDelta);
		if (NextTargetDelta < TargetDelta) {
			Min = NextLengthAlongSpline;
			LengthAlongSpline = NextLengthAlongSpline;
			LocationOnSpline = NextLocationOnSpline;
			TargetDelta = NextTargetDelta;

		} else {
			Max = NextLengthAlongSpline;
		}
	}




	return LocationOnSpline;
}

void USplineHelpers::SplineData(USplineComponent* Spline, const FVector& TargetLocation, const float& Distance, FVector& OutLocationOnSpline, float& OutDistanceFromTarget) {
	OutLocationOnSpline = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	OutDistanceFromTarget = FVector::Distance(TargetLocation, OutLocationOnSpline);
}