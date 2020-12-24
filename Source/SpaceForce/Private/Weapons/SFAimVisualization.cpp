#include "Weapons/SFAimVisualization.h"

ASFAimVisualization::ASFAimVisualization()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_EndPhysics;
}

bool ASFAimVisualization::GetTarget(FVector& OutVector) const
{
	if (bTargetLocationSet)
	{
		OutVector = Target;
		return true;
	}
	if (TargetComp)
	{
		OutVector = TargetComp->GetComponentLocation();
		return true;
	}
	return false;
}

void ASFAimVisualization::SetTargetComponent(USceneComponent* SceneComponent)
{
	bTargetLocationSet = false;
	TargetComp = SceneComponent;
}

void ASFAimVisualization::SetTargetLocation(FVector Value)
{
	bTargetLocationSet = true;
	Target = Value;
}

void ASFAimVisualization::ClearTarget()
{
	bTargetLocationSet = false;
	Target = FVector::ZeroVector;
	TargetComp = NULL;
}

void ASFAimVisualization::SetActivated(bool bValue)
{
	ReceiveSetActivated(bValue);
}