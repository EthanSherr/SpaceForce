// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SplineHelpers.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API USplineHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector ClosestPointOnSplineToLocation(class USplineComponent* SplineComponent, FVector Location, float Min = 0.0f, float Max = -1.0f, int MaxIterations = 10, bool bDebug = true);
private:
	static void SplineData(class USplineComponent* Spline, const FVector& TargetLocation, const float& Distance,FVector& OutLocationOnSpline, float& OutDistanceFromTarget);
};
