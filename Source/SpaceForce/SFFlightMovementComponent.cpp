// Fill out your copyright notice in the Description page of Project Settings.


#include "SFFlightMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "SFCollisionDetector.h"

USFFlightMovementComponent::USFFlightMovementComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxRotationSpeed = FRotator(35);
	MaxSpeed = 300.0f;
	MinSpeed = 200.0f;
	Acceleration = 30.0f;
	Deceleration = 60.0f;
	bDrawDebugLines = true;
	bAdjustRollToFavorPitch = true;
}

void USFFlightMovementComponent::InitializeCollisionDetection()
{
	CollisionDetector = (USFCollisionDetector*)GetOwner()->GetComponentByClass(USFCollisionDetector::StaticClass());
}

void USFFlightMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	DeltaRotation = FRotator::ZeroRotator;
	InitializeCollisionDetection();
	currentSpeed = MinSpeed;
}

void USFFlightMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bHasTargetPoint)
	{
		MoveTowardsTarget(targetPoint);
	}

	FRotator RotationVelocity = FRotator(
		MaxRotationSpeed.Pitch * DeltaRotation.Pitch,
		MaxRotationSpeed.Yaw * DeltaRotation.Yaw,
		MaxRotationSpeed.Roll * DeltaRotation.Roll);
	GetOwner()->AddActorLocalRotation(DeltaTime * RotationVelocity);

	UE_LOG(LogTemp, Warning, TEXT("dthrust %f from %f"), (DeltaTime * thrust * (thrust > 0 ? Acceleration : -Deceleration)), thrust)
	currentSpeed += DeltaTime * thrust * (thrust > 0 ? Acceleration : -Deceleration);
	currentSpeed = FMath::Max(FMath::Min(currentSpeed, MaxSpeed), MinSpeed);

	UE_LOG(LogTemp, Warning, TEXT("speed %f, DeltaRotation %s"), currentSpeed, *DeltaRotation.ToString())
	FVector LastLocation = GetOwner()->GetActorLocation();
	GetOwner()->AddActorLocalOffset(DeltaTime * currentSpeed * FVector::ForwardVector);
	if (bDrawDebugLines) {
		DrawDebugLine(GetWorld(), LastLocation, GetOwner()->GetActorLocation(), FColor::Black, false, 60, 3, 1);
		//DrawDebugLine(GetWorld(), LastLocation, GetOwner()->GetActorUpVector() * 50 + LastLocation, FColor::Red, false, 60, 3, 0.5);
	}
	DeltaRotation = FRotator::ZeroRotator;
}

void USFFlightMovementComponent::AddInputVector(FRotator rotation)
{
	rotation.Roll = FMath::Min(FMath::Max(rotation.Roll, -1.0f), 1.0f);
	rotation.Pitch = FMath::Min(FMath::Max(rotation.Pitch, -1.0f), 1.0f);
	rotation.Yaw = FMath::Min(FMath::Max(rotation.Yaw, -1.0f), 1.0f);
	DeltaRotation = rotation;
}

void USFFlightMovementComponent::MoveTo(FVector targetWorldLocation)
{
	bHasTargetPoint = true;
	targetPoint = targetWorldLocation;
}

void USFFlightMovementComponent::MoveTowardsTarget(FVector targetWorldLocation)
{
	FCollisionDetectionResult collisionDetectorResult;
	if (CollisionDetector != NULL) {
		collisionDetectorResult = CollisionDetector->DetectCollisions();
	}

	FVector targetRelLoc = GetOwner()->GetActorRotation().UnrotateVector(targetWorldLocation - GetOwner()->GetActorLocation());
	FRotator targetRelRot = FRotationMatrix::MakeFromX(targetRelLoc).Rotator();

	if (collisionDetectorResult.bCollisionDetected)
	{
		if (FMath::Sign(collisionDetectorResult.SuggestedDeltaRotation.Yaw) !=
			FMath::Sign(targetRelRot.Yaw)) {
			targetRelRot.Yaw = collisionDetectorResult.SuggestedDeltaRotation.Yaw;
		}
		if (FMath::Sign(collisionDetectorResult.SuggestedDeltaRotation.Pitch) !=
			FMath::Sign(targetRelRot.Pitch)) {
			targetRelRot.Pitch = collisionDetectorResult.SuggestedDeltaRotation.Pitch;
		}
	}

	if (bAdjustRollToFavorPitch && FMath::Abs(targetRelRot.Yaw) > FMath::Abs(targetRelRot.Pitch)) {
		targetRelRot.Roll = targetRelRot.Yaw;
	}

	AddInputVector(targetRelRot);
	SetTargetThrust(1 - collisionDetectorResult.NormalizedSignalStrength/0.5);
}

void USFFlightMovementComponent::SetTargetThrust(float value)
{
	thrust = FMath::Max(FMath::Min(value, 1.0f), -1.0f);
}