// Fill out your copyright notice in the Description page of Project Settings.


#include "SFFlightMovementComponent.h"
#include "GameFramework/Actor.h"

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
	GetOwner()->AddActorLocalOffset(DeltaTime * MaxSpeed * FVector::ForwardVector);
	DeltaRotation = FRotator::ZeroRotator;
}

void USFFlightMovementComponent::AddInputVector(FRotator rotation)
{
	DeltaRotation += rotation;
}

