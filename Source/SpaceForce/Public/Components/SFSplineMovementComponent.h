// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SFSplineMovementComponent.generated.h"

class USplineComponent;
class ASFFlightPath;

UCLASS( ClassGroup=Movement, meta=(BlueprintSpawnableComponent), ShowCategories=(Velocity) )
class SPACEFORCE_API USFSplineMovementComponent : public UPawnMovementComponent
{
	GENERATED_UCLASS_BODY()

public:	
	// Begin UActorComponent
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End UActorComponentInterface

	// Begin UMovementComponent
	virtual float GetMaxSpeed() const override { return Speed; }
	virtual void InitializeComponent() override;
	virtual void UpdateTickRegistration() override;
	// End UMovementComponent Interface

	UFUNCTION(BlueprintCallable)
	void SetFlightPath(ASFFlightPath* FlightPath, float distanceAlongPath);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFFlightPath* GetFlightPath();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseSpeed;

	UPROPERTY(BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere)
	bool bOrientToSpline;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetLocationAtDistanceAlongSpline(float distance, bool bWithOffset);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetDirectionAtDistanceAlongSpline(float distance);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetDistance() { return distanceAlongPath; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FVector GetLocation() { return GetLocationAtDistanceAlongSpline(GetDistance(), false); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const FVector GetDirection() { return GetDirectionAtDistanceAlongSpline(GetDistance()); }

	UPROPERTY(BlueprintReadWrite)
	ASFFlightPath* NextFlightPath;

	UFUNCTION()
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float initialOffset = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bVerboseDelta;

private:

	UPROPERTY(EditAnywhere)
	ASFFlightPath* FlightPath;

	UPROPERTY()
	float distanceAlongPath;

	FVector LastLocation;

		
};
