#include "AI/SFTurretControllerManager.h"
#include "AI/SFTurretController.h"
#include "Weapons/SFTurretActor.h"
#include "Components/SFTracker.h"

USFTurretControllerManager::USFTurretControllerManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void USFTurretControllerManager::Initialize(USFTracker* Tracker)
{
	TrackerRef = Tracker;
	for (USFTurretController* Controller : Controllers)
	{
		if (!Controller)
		{
			UE_LOG(LogTemp, Error, TEXT("USFTurretControllerManager %s : Controllers contains null ptr"), *GetOwner()->GetName())
			continue;
		}
		Controller->Initialize(this);
	}
}

void USFTurretControllerManager::RegisterTurret(int ControllerId, ASFTurretActor* Turret)
{
	if (!Controllers.IsValidIndex(ControllerId))
	{
		UE_LOG(LogTemp, Error, TEXT("SFTurretControllerManager Error RegisterTurret : %s not a valid TurretController at index %d"), *GetOwner()->GetName(), ControllerId)
		return;
	}
	USFTurretController* Controller = Controllers[ControllerId];
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("SFTurretControllerManager Error RegisterTurret : %s has no TurretController at index %d"), *GetOwner()->GetName(), ControllerId)
		return;
	}

	Controller->RegisterTurret(Turret);
}

void USFTurretControllerManager::SwitchController(int ControllerId)
{
	if (!GetTracker())
	{
		UE_LOG(LogTemp, Error, TEXT("SFTurretControllerManager Error : Invalid Tracker - %s must call Initialize in/after Super::PostInitComonents()"), *GetOwner()->GetName());
		return;
	}
	//DeactivateAll();
	if (!Controllers.IsValidIndex(ControllerId))
	{
		UE_LOG(LogTemp, Error, TEXT("SFTurretControllerManager SwitchController Error : Invalid index for controller at %d"), ControllerId)
		return;
	}
	if (USFTurretController* NewController = Controllers[ControllerId])
	{
		ActiveControllers.Add(ControllerId);
		NewController->SetIsActive(true);
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("SFTurretControllerManager Error SwitchController : %s has no TurretController at index %d"), *GetOwner()->GetName(), ControllerId)
}

void USFTurretControllerManager::DeactivateAll()
{
	for (uint16 ActiveIndex : ActiveControllers)
	{
		Controllers[ActiveIndex]->SetIsActive(false);
	}
	ActiveControllers.Empty();
}

// Called every frame
void USFTurretControllerManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

USFTracker* USFTurretControllerManager::GetTracker() const
{
	return TrackerRef.Get();
}

bool USFTurretControllerManager::GetControllers(TArray<USFTurretController*>& OutControllers)
{
	OutControllers = Controllers;
	return Controllers.Num() > 0;
}