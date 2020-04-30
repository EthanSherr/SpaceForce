// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFCollisionDetector.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct SPACEFORCE_API FCollisionSignal {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Strength;

	UPROPERTY(BlueprintReadWrite)
	FVector SensorWorld;

	FCollisionSignal() : Strength(0.0f), SensorWorld(FVector::ZeroVector) {}
	FCollisionSignal(float strength, FVector worldSensor) : Strength(strength), SensorWorld(worldSensor) {}
};


USTRUCT(BlueprintType)
struct SPACEFORCE_API FCollisionDetectionResult {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bCollisionDetected;

	UPROPERTY(BlueprintReadWrite)
	FVector CollisionVector;

	UPROPERTY(BlueprintReadWrite)
	FRotator SuggestedDeltaRotation;

	UPROPERTY(BlueprintReadWrite)
	float NormalizedSignalStrength;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCollisionSignal> CollisionSignals;

	FCollisionDetectionResult() 
		: bCollisionDetected(false), CollisionVector(FVector::ZeroVector), SuggestedDeltaRotation(FRotator::ZeroRotator), NormalizedSignalStrength(0.0f) {}

	FCollisionDetectionResult(bool _bCollisionDetected, FVector _CollisionVector, FRotator _SuggestedDeltaRotation, float _NormalizedSignalStrength, TArray<FCollisionSignal> _CollisionSignals)
		: bCollisionDetected(_bCollisionDetected), CollisionVector(_CollisionVector), 
		SuggestedDeltaRotation(_SuggestedDeltaRotation), NormalizedSignalStrength(_NormalizedSignalStrength), CollisionSignals(_CollisionSignals)
		{}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFCollisionDetector : public UActorComponent
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	bool bDebugTraces;

	UPROPERTY(EditAnywhere)
	int GridDimension;

	UPROPERTY(EditAnywhere)
	float SensorSpacing;

	UPROPERTY(EditAnywhere)
	float SensorDistanceFromOrigin;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ResponseCurve;

	UPROPERTY()
	TArray<FVector> SensorPoints;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FCollisionDetectionResult DetectCollisions();

};
