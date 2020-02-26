// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FProjectilePredictionResult
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bSuccess;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float time;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector projectileVelocity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector predictedImpact;

	FProjectilePredictionResult() {
		bSuccess = false;
		time = -1.f;
		projectileVelocity = FVector::ZeroVector;
		predictedImpact = FVector::ZeroVector;
	}
};

UCLASS()
class SPACEFORCE_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MyCategory")
		static FProjectilePredictionResult ComputeProjectilePrediction(FVector targetPosition, FVector targetVelocity, FVector projectileOrigin, float projectileSpeed, float muzzleOffsetMagnitudeFromProjectileOrigin);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MyCategory")
		static float MinNonNegative(float a, float b);
};