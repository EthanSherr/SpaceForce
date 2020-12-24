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
		SetTargetActor(InitialTrackedActor);
	}
}

void USFTracker::SetTargetVector(FVector Vector)
{
	ClearTarget();
	TrackedVector = Vector;
	TrackerType == ETrackerType::Vector;
}

void USFTracker::SetTargetComponent(USceneComponent* Component)
{
	ClearTarget();
	if (Component && Component->IsValidLowLevel())
	{
		TrackedComponent = Component;
		TrackerType = ETrackerType::Component;
	}
}

void USFTracker::SetTargetActor(AActor* Actor)
{
	ClearTarget();
	if (Actor && Actor->IsValidLowLevel())
	{
		SetTargetComponent(Actor->GetRootComponent());
	}
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
			bSuccess = true;
		}
	}
	else
	if (TrackerType == ETrackerType::Vector)
	{
		OutPosition = TrackedVector;
		bSuccess = true;
	}
	return bSuccess;
}

void USFTracker::ClearTarget()
{
	if (USceneComponent* Comp = TrackedComponent.Get())
	{
		TrackedComponent = NULL;
	}
	TrackerType = ETrackerType::None;
}