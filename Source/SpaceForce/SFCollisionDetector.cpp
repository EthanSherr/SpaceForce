// Fill out your copyright notice in the Description page of Project Settings.


#include "SFCollisionDetector.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Curves/CurveFloat.h"
#include "DrawDebugHelpers.h"

USFCollisionDetector::USFCollisionDetector(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	GridDimension = 3;
	SensorSpacing = 75.0f;
	SensorDistanceFromOrigin = 450.0f;
	bDebugTraces = true;
	ResponseCurve = NULL;
}

void USFCollisionDetector::BeginPlay()
{
	Super::BeginPlay();
	LastCollisionSignal = FCollisionSignal();
	int sensorPointsSize = GridDimension * GridDimension;
	SensorPoints.SetNum(sensorPointsSize);
	for (int i = 0; i < sensorPointsSize; i++) {
		int y = i % GridDimension - GridDimension / 2;
		int z = i / GridDimension - GridDimension / 2;

		FVector sensorGridPoint = FVector(SensorDistanceFromOrigin, y * SensorSpacing, z * SensorSpacing);
		FVector sensorPoint = SensorDistanceFromOrigin * sensorGridPoint.GetSafeNormal();
		SensorPoints[i] = sensorPoint;
	}
}

FCollisionDetectionResult USFCollisionDetector::DetectCollisions()
{
	FVector actorLocation = GetOwner()->GetActorLocation();
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	TArray<FCollisionSignal> signalStrengths;
	signalStrengths.Init(FCollisionSignal(), SensorPoints.Num());
	bool bCollisionDetected = false;
	for (int i = 0; i < SensorPoints.Num(); i++)
	{
		FVector point = SensorPoints[i];
		FVector sensorLocation = GetOwner()->GetActorRotation().RotateVector(point) + actorLocation;

		FHitResult outHit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(outHit, actorLocation, sensorLocation, ECollisionChannel::ECC_Visibility, params);
		float responseStrength = 0.0f;
		if (bHit)
		{
			FVector delta = outHit.Location - actorLocation;
			float impactDistance = delta.Size();
			float maxDistance = point.Size();
			float a = 1 - (maxDistance - impactDistance) / maxDistance;
			responseStrength = ResponseCurve ? ResponseCurve->GetFloatValue(a) : 1.0f;
			bCollisionDetected = true;
		}
		signalStrengths[i].Index = i;
		signalStrengths[i].Strength = responseStrength;

		if (bDebugTraces)
		{
			DrawDebugLine(GetWorld(), actorLocation, bHit ? outHit.Location : sensorLocation, (bHit ? FColor::Red : FColor::Green), false, 0, 1, 0.5f);
			if (bHit) {
				DrawDebugSphere(GetWorld(), outHit.Location, 5, 5, FColor::Red, false, 0, 1, 0.5f);
			}
			if (responseStrength > 0.0f) {
				FVector end = actorLocation + (sensorLocation - actorLocation).GetSafeNormal() * responseStrength * 100;
				DrawDebugLine(GetWorld(), actorLocation, end, FColor::Purple, false, 0, 2, 0.8f);
			}
		}
	}

	if (bCollisionDetected)
	{
		signalStrengths.Sort([](const FCollisionSignal& a, const FCollisionSignal& b) { return a.Strength < b.Strength; });
	}
	else {
		LastCollisionSignal = FCollisionSignal();
	}

	UE_LOG(LogTemp, Warning, TEXT("-----"))
	for (FCollisionSignal signal : signalStrengths)
	{
		UE_LOG(LogTemp, Warning, TEXT("strength[%d] %f"), signal.Index, signal.Strength)
	}


	FCollisionDetectionResult result = FCollisionDetectionResult();
	result.bCollisionDetected = bCollisionDetected;
	//result.AvoidanceResponse = responseVector;
	return result;
}



