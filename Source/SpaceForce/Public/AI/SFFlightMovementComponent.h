// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFFlightMovementComponent.generated.h"

class USFCollisionDetector;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFFlightMovementComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	
	USFFlightMovementComponent();

protected:
	virtual void BeginPlay() override;
	void InitializeCollisionDetection();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	bool bDrawDebugLines;

	UPROPERTY(EditAnywhere)
	bool bAdjustRollToFavorPitch;

	UPROPERTY(EditAnywhere)
	FRotator MaxRotationSpeed;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	UPROPERTY(EditAnywhere)
	float MinSpeed;

	UPROPERTY(EditAnywhere)
	float Deceleration;

	UPROPERTY(EditAnywhere)
	float Acceleration;

	UFUNCTION(BlueprintCallable)
	void AddInputVector(FRotator rotation);

	UFUNCTION(BlueprintCallable)
	void MoveTo(FVector worldLocation);

	UFUNCTION(BlueprintCallable)
	void SetTargetThrust(float value);

	UPROPERTY(EditAnywhere)
	bool bEnabled;

	UFUNCTION(BlueprintCallable)
		void SetEnabled(bool val);

private:
	FRotator DeltaRotation;

	USFCollisionDetector* CollisionDetector;

	float thrust;
	float currentSpeed;

	FVector targetPoint;
	bool bHasTargetPoint;

	UFUNCTION()
	void MoveTowardsTarget(FVector worldTarget);

};
