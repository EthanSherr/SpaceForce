// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SplineMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFFlightPath.generated.h"

class USplineComponent;

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

private:
	void SetupSpline();

};
