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

	UFUNCTION(BlueprintCallable)
	void SetFlightPath(ASFFlightPath* FlightPath, float distanceAlongPath);

	UFUNCTION(BlueprintCallable)
	void SetNextFlightPath(ASFFlightPath* FlightPath);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFFlightPath* GetFlightPath();

	UPROPERTY(EditAnywhere, BlueprintSetter=SetSpeed, BlueprintGetter=GetSpeed)
	float speed;

	UFUNCTION(BlueprintCallable, BlueprintSetter)
	void SetSpeed(float s);

	UFUNCTION(BlueprintCallable, BlueprintGetter)
	float GetSpeed();

	UPROPERTY(EditAnywhere)
	bool bOrientToSpline;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetLocationAtDistanceAlongSpline(float distance, bool bWithOffset);

	UPROPERTY()
	ASFFlightPath* NextFlightPath;

	UFUNCTION()
	virtual void InitializeComponent() override;

	UFUNCTION()
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		float initialOffset = 0.0f;


private:

	UPROPERTY(EditAnywhere)
	ASFFlightPath* FlightPath;

	UPROPERTY()
	float distanceAlongPath;

	FVector LastLocation;

		
};
