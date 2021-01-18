#include "AI/SFTurretController.h"
#include "AI/SFTurretControllerManager.h"
#include "Weapons/SFTurretActor.h"
#include "Helpers/SFMath.h"
#include "Components/SFTracker.h"
#include "DrawDebugHelpers.h"

USFTurretController::USFTurretController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNeedsTick = false;
}

void USFTurretController::Initialize(USFTurretControllerManager* ControllerManager)
{
	ManagerRef = ControllerManager;
	UE_LOG(LogTemp, Warning, TEXT("TurretController %s initialized"), *GetName())
}

void USFTurretController::RegisterTurret(ASFTurretActor* Turret)
{
	TWeakObjectPtr<ASFTurretActor> TurretRef = Turret;
	Turrets.Add(TurretRef);
}

bool USFTurretController::GetIsActive() const
{
	return bIsActive;
}

USFTracker* USFTurretController::GetTracker() const
{
	if (USFTurretControllerManager* Manager = ManagerRef.Get())
	{
		return Manager->GetTracker();
	}
	return NULL;
}

APawn* USFTurretController::GetPawn() const
{
	if (USFTurretControllerManager* Manager = ManagerRef.Get())
	{
		return Cast<APawn>(Manager->GetOwner());
	}
	return NULL;
}

bool USFTurretController::SetIsActive(bool bValue)
{
	if (bIsActive == bValue)
	{
		return false;
	}

	bIsActive = bValue;
	for (TWeakObjectPtr<ASFTurretActor> TurretRef : Turrets)
	{
		ASFTurretActor* Turret = TurretRef.Get();
		if (Turret && Turret->IsValidLowLevel())
		{
			if (bIsActive)
			{
				Turret->DelegateRef = this;
				Turret->SetActivated(true);
			} else if (Turret->DelegateRef.Get() == this)
			{
				Turret->DelegateRef = NULL;
				Turret->SetActivated(false);
			}
		}
	}

	if (bIsActive)
	{
		OnActivate();
	}
	else
	{
		OnDeactivate();
	}

	return true;
}
// Some typicall helpers
bool USFTurretController::LeadTarget(ASFTurretActor* Turret, FVector& OutTarget)
{
	USFTracker* EnemyTracker = GetTracker();
	FVector Position;
	FVector Velocity;
	if (!EnemyTracker || !EnemyTracker->GetTarget(Position, Velocity))
	{
		return false;
	}
	FProjectilePredictionResult Result = USFMath::ComputeProjectilePrediction(
		Position,
		Velocity,
		Turret->GetBarrelTransform().GetLocation(),
		Turret->GetProjectileSpeed(),
		Turret->GetBarrelLength()
	);

	OutTarget = Result.predictedImpact;
	return true;
}

// Override API
bool USFTurretController::OnActivate_Implementation()
{
	return false;
}

bool USFTurretController::OnDeactivate_Implementation()
{
	return false;
}

// TurretDelegate
bool USFTurretController::GetTarget_Implementation(ASFTurretActor* Turret, float DeltaSeconds, FVector& OutVector)
{
	return LeadTarget(Turret, OutVector);
}

bool USFTurretController::GetTurrets(TArray<ASFTurretActor*>& OutTurrets)
{
	TArray<ASFTurretActor*> Arr;
	for (TWeakObjectPtr<ASFTurretActor> TurretRef : Turrets)
	{
		if (ASFTurretActor* Turret = TurretRef.Get())
		{
			Arr.Add(Turret);
		}
	}
	OutTurrets = Arr;
	return Arr.Num() > 0;
}

UWorld* USFTurretController::GetWorld() const
{
	if (GetOuter() == nullptr)
	{
		return nullptr;
	}

	// Special case for behavior tree nodes in the editor
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		// GetOuter should return a UPackage and its Outer is a UWorld
		return Cast<UWorld>(GetOuter()->GetOuter());
	}

	// In all other cases...
	return GetOuter()->GetWorld();
}

void USFTurretController::BroadcastLoopFinished()
{
	LoopFinished.Broadcast();
}