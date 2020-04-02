// Fill out your copyright notice in the Description page of Project Settings.


#include "SFFlightMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

USFFlightMovementComponent::USFFlightMovementComponent(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxRotationSpeed = 5.0f;
	MaxSpeed = 50.0f;
}

void USFFlightMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	DeltaRotation = FRotator::ZeroRotator;
}

void USFFlightMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GetOwner()->AddActorLocalRotation(DeltaTime * MaxRotationSpeed * DeltaRotation);
	UE_LOG(LogTemp, Warning, TEXT("DeltaRotation %s"), *DeltaRotation.ToString())
	FVector LastLocation = GetOwner()->GetActorLocation();
	GetOwner()->AddActorLocalOffset(DeltaTime * MaxSpeed * FVector::ForwardVector);
	DrawDebugLine(GetWorld(), LastLocation, GetOwner()->GetActorLocation(), FColor::Black, false, 60, 3, 1);
	DeltaRotation = FRotator::ZeroRotator;
}

void USFFlightMovementComponent::AddInputVector(FRotator rotation)
{
	rotation.Roll = FMath::Clamp(rotation.Roll, -1.0f, 1.0f);
	rotation.Pitch = FMath::Clamp(rotation.Pitch, -1.0f, 1.0f);
	rotation.Yaw = FMath::Clamp(rotation.Yaw, -1.0f, 1.0f);
	DeltaRotation = (DeltaRotation + rotation) * 0.5f;
}

