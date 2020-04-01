// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFCollisionDetector.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct SPACEFORCE_API FCollisionDetectionResult {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bCollisionDetected;

	UPROPERTY(BlueprintReadWrite)
	FVector CollisionVector;

	UPROPERTY(BlueprintReadWrite)
	FRotator SuggestedDeltaRotation;

	FCollisionDetectionResult() 
		: bCollisionDetected(false), CollisionVector(FVector::ZeroVector), SuggestedDeltaRotation(FRotator::ZeroRotator) {}

	FCollisionDetectionResult(bool _bCollisionDetected, FVector _CollisionVector, FRotator _SuggestedDeltaRotation)
		: bCollisionDetected(_bCollisionDetected), CollisionVector(_CollisionVector), SuggestedDeltaRotation(_SuggestedDeltaRotation) {}
};

USTRUCT()
struct SPACEFORCE_API FCollisionSignal {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Strength;

	UPROPERTY()
	int Index;

	FCollisionSignal() : Strength(0.0f), Index(-1) {}
	FCollisionSignal(float strength, int index) : Strength(strength), Index(index) {}
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

private:
	void DebugStrengths(TArray<FCollisionSignal> strengths);

};
