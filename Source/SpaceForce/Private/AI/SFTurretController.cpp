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
	if (Result.bSuccess)
	{
		OutTarget = Result.predictedImpact;
		DrawDebugPoint(GetWorld(), OutTarget, 20, FColor::Purple, false, 0.0f, 5);
	}

	FString Message = FString::Printf(TEXT("pos %s, velocity %s, Turret %s speed %f, length %f"),
		*Position.ToString(), *Velocity.ToString(), *Turret->GetBarrelTransform().GetLocation().ToString(), Turret->GetProjectileSpeed(), Turret->GetBarrelLength());
	GEngine->AddOnScreenDebugMessage(11, 0.0f, Result.bSuccess ? FColor::Green : FColor::Red, *Message);

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
bool USFTurretController::GetTarget_Implementation(ASFTurretActor* Turret, FVector& OutVector)
{
	return LeadTarget(Turret, OutVector);
}

TArray<ASFTurretActor*> USFTurretController::GetTurrets() const
{
	TArray<ASFTurretActor*> Arr;
	for (TWeakObjectPtr<ASFTurretActor> TurretRef : Turrets)
	{
		if (ASFTurretActor* Turret = TurretRef.Get())
		{
			Arr.Add(Turret);
		}
	}
	return Arr;
}