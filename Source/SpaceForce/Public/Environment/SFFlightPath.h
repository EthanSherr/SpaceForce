// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SplineMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFFlightPath.generated.h"

class USplineComponent;
class USphereComponent;

USTRUCT(BlueprintType)
struct SPACEFORCE_API FSplineDistance {
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float Distance;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FVector Forward;

	FORCEINLINE FSplineDistance() {
		Distance = 0.0f;
		Location = FVector::ZeroVector;
		Forward = FVector::ZeroVector;
	};

	FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("Distance: %f, Location: (%s), Forward: (%s)"), Distance, *Location.ToString(), *Forward.ToString());
	}
};

UCLASS()
class SPACEFORCE_API ASFFlightPath : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	USplineComponent* Spline;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintPure)
	FVector GetLocationAtDistance(float distance);

	UPROPERTY(EditAnywhere, Category = "CustomSmoothing")
	bool bUseCustomInterpolation;

	UPROPERTY(EditAnywhere, Category = "CustomSmoothing", meta = (EditCondition = "bUseCustomInterpolation"))
	float SplineStepSize;

	UPROPERTY(EditAnywhere)
	bool bPathForPlayer;

	UPROPERTY()
	USphereComponent* BeginSphere;

private:

	void SetupStartSphere();
	void SetupSpline();

	UPROPERTY()
	TArray<FSplineDistance> Points;

	bool PointsForDistance(float Distance, FSplineDistance& OutStart, FSplineDistance& OutEnd);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector FindLocationClosestToWorldLocation(FVector Location);

};
