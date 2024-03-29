// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SFMath.generated.h"

USTRUCT(BlueprintType)
struct FProjectilePredictionResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSuccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector predictedImpact;

	FProjectilePredictionResult() {
		bSuccess = false;
		time = -1.f;
	}
};

UCLASS()
class SPACEFORCE_API USFMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MyCategory")
	static FProjectilePredictionResult ComputeProjectilePrediction(FVector targetPosition, FVector targetVelocity, FVector projectileOrigin, float projectileSpeed, float muzzleOffsetMagnitudeFromProjectileOrigin);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Float math")
	static float MinMax(float v, float min, float max);

private:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MyCategory")
	static float MinNonNegative(float a, float b);
};