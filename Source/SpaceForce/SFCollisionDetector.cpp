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
	SensorSpacing = 25.0f;
	SensorDistanceFromOrigin = 450.0f;
	bDebugTraces = true;
}

void USFCollisionDetector::BeginPlay()
{
	Super::BeginPlay();
	int sensorPointsSize = GridDimension * GridDimension;
	SensorPoints.SetNum(sensorPointsSize);
	for (int i = 0; i < sensorPointsSize; i++) {
		int y = i % GridDimension - GridDimension / 2;
		int z = i / GridDimension - GridDimension / 2;

		FVector sensorGridPoint = FRotator(y * SensorSpacing, z * SensorSpacing, 0).RotateVector(FVector::ForwardVector);
		FVector sensorPoint = SensorDistanceFromOrigin * sensorGridPoint;
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
	FVector collisionVector = FVector::ZeroVector;
	for (int i = 0; i < SensorPoints.Num(); i++)
	{
		FVector point = SensorPoints[i];
		FVector sensorLocation = GetOwner()->GetActorRotation().RotateVector(point) + actorLocation;

		FHitResult outHit;
		float responseStrength = 0.0f;
		bool bHit = GetWorld()->LineTraceSingleByChannel(outHit, actorLocation, sensorLocation, ECollisionChannel::ECC_Visibility, params);
		if (bHit)
		{
			FVector delta = outHit.Location - actorLocation;
			float impactDistance = delta.Size();
			float maxDistance = point.Size();
			float a = 1 - (maxDistance - impactDistance) / maxDistance;
			responseStrength = ResponseCurve ? ResponseCurve->GetFloatValue(a) : 1.0f;
			collisionVector += responseStrength * point.GetSafeNormal();
			bCollisionDetected = true;
		}
		signalStrengths[i].Index = i;
		signalStrengths[i].Strength = responseStrength;

		if (bDebugTraces)
		{
			DrawDebugLine(GetWorld(), actorLocation, bHit ? outHit.Location : sensorLocation, (bHit ? FColor::Red : FColor::Green), false, 0, 1, 0.5f);
			if (bHit) {
				DrawDebugSphere(GetWorld(), outHit.Location, 3, 2, FColor::Red, false, 0, 1, 0.5f);
			}
			if (responseStrength > 0.0f) {
				FVector end = actorLocation + (sensorLocation - actorLocation).GetSafeNormal() * responseStrength * 100;
				DrawDebugLine(GetWorld(), actorLocation, end, FColor::Purple, false, 0, 2, 0.8f);
			}
		}
	}

	collisionVector /= SensorPoints.Num();

	FRotator suggestedOrientation = FRotator::ZeroRotator;
	if (bCollisionDetected)
	{
		suggestedOrientation.Yaw = collisionVector.Y <= 0.0f ? 1 : -1;
		suggestedOrientation.Pitch = collisionVector.Z <= 0.0f ? 1 : -1;
		if (bDebugTraces) {
			DrawDebugLine(GetWorld(), actorLocation, 10 * collisionVector + actorLocation, FColor::Blue, false, 0, 3, 1);
		}
		suggestedOrientation *= collisionVector.Size();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Pitch, Yaw (%f, %f) collsionVector |(%s)| = %f"), suggestedOrientation.Pitch, suggestedOrientation.Yaw, *collisionVector.ToString(), collisionVector.Size())
	FCollisionDetectionResult result = FCollisionDetectionResult(bCollisionDetected, collisionVector, suggestedOrientation);
	return result;
}

void USFCollisionDetector::DebugStrengths(TArray<FCollisionSignal> str)
{
	UE_LOG(LogTemp, Warning, TEXT("Signals \n"))
}



