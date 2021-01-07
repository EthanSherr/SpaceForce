#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFTurretControllerManager.generated.h"

class ASFTurretActor;
class USFTurretController;
class USFTracker;

UCLASS( ClassGroup=(TurretController), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFTurretControllerManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	USFTurretControllerManager(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Initialize(USFTracker* Tracker);

	UFUNCTION(BlueprintCallable)
	void RegisterTurret(int ControllerId, ASFTurretActor* Turret);

	UFUNCTION(BlueprintCallable, Category = "Controller Management")
	void SwitchController(int ControllerId);

	UFUNCTION(BlueprintCallable, Category = "Controller Management")
	void DeactivateAll();

	USFTracker* GetTracker() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetControllers(TArray<USFTurretController*>& OutControllers);

protected:

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Initialization")
	TArray<USFTurretController*> Controllers;

	UPROPERTY(Transient)
	TArray<uint16> ActiveControllers;

	UPROPERTY(Transient)
	TWeakObjectPtr<USFTracker> TrackerRef;

};
