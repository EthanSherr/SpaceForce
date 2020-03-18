// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFSplineMovementComponent.generated.h"

class USplineComponent;
class ASFFlightPath;

UCLASS( ClassGroup="Custom", meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFSplineMovementComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintSetter)
	void SetFlightPath(ASFFlightPath* FlightPath, float distanceAlongPath);

	UPROPERTY(EditAnywhere)
	float speed;

	UPROPERTY(EditAnywhere)
	bool bOrientToSpline;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetLocationAtDistanceAlongSpline(float distance, bool bWithOffset);

private:

	UPROPERTY(EditAnywhere)
	ASFFlightPath* FlightPath;

	UPROPERTY()
	float distanceAlongPath;

		
};
