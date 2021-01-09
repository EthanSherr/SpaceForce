#pragma once
#include "SFTracker.h"

USFTracker::USFTracker(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TrackerType = ETrackerType::None;
	PrimaryComponentTick.bCanEverTick = false;
}

void USFTracker::BeginPlay()
{
	Super::BeginPlay();
	if (InitialTrackedActor)
	{
		bool OutChanged;
		SetTargetActor(InitialTrackedActor, OutChanged);
	}
}

void USFTracker::SetTargetVector(FVector Vector)
{
	TrackedVector = Vector;
	TrackerType == ETrackerType::Vector;
}

void USFTracker::SetTargetComponent(USceneComponent* Component, bool& OutTargetChanged)
{
	USceneComponent* NewComponent = Component && Component->IsValidLowLevel() ? Component : NULL;
	OutTargetChanged = TrackedComponent.Get() != NewComponent || ETrackerType::Component != TrackerType;
	if (OutTargetChanged)
	{
		TrackedComponent = NewComponent;
		TrackerType = ETrackerType::Component;
	}
}

void USFTracker::SetTargetActor(AActor* Actor, bool& OutTargetChanged)
{
	USceneComponent* NewCompnent = Actor && Actor->IsValidLowLevel() ? Actor->GetRootComponent() : NULL;
	SetTargetComponent(NewCompnent, OutTargetChanged);
}

bool USFTracker::GetTarget(FVector& OutPosition, FVector& OutVelocity)
{
	if (TrackerType == ETrackerType::None)
	{
		return false;
	}

	bool bSuccess = false;
	if (TrackerType == ETrackerType::Component)
	{
		USceneComponent* SceneComp = TrackedComponent.Get();
		if (SceneComp && SceneComp->IsValidLowLevel())
		{
			OutPosition = SceneComp->GetComponentLocation();
			OutVelocity = SceneComp->GetComponentVelocity();
			bSuccess = true;
		}
	}
	else
	if (TrackerType == ETrackerType::Vector)
	{
		OutPosition = TrackedVector;
		UE_LOG(LogTemp, Error, TEXT("USFTracker GetTarget Error: Velocity is not yet tracked when ETrackerType::Vector"))
		bSuccess = true;
	}
	return bSuccess;
}

bool USFTracker::HasTarget() const
{
	return TrackerType != ETrackerType::None;
}

bool USFTracker::HalfSpaceDistance(const FVector& ComparedLocation, float& OutDistance, float MinimalStableSpeed)
{
	FVector TrackedLocation;
	FVector TrackedVelocity;
	if (!GetTarget(TrackedLocation, TrackedVelocity)) return false;

	if (TrackerType == ETrackerType::Component)
	{
		OutDistance = FVector::DotProduct(ComparedLocation - TrackedLocation, TrackedComponent->GetForwardVector());
		return true;
	}
	else
	if (TrackerType == ETrackerType::Vector)
	{
		//TODO use velocity and MinimalStableSpeed here
		UE_LOG(LogTemp, Error, TEXT("USFTracker Error : HalfSpaceDistance doesn't know how to use forward vector of ETrackerType::Vector yet, use velocity!"))
		return false;
	}
	return false;
}

void USFTracker::ClearTarget()
{
	if (USceneComponent* Comp = TrackedComponent.Get())
	{
		TrackedComponent = NULL;
	}
	TrackerType = ETrackerType::None;
}
