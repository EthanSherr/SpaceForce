#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/SFTurretDelegate.h"
#include "SFTurretController.generated.h"

class APawn;
class ASFTurretActor;
class USFTurretControllerManager;
class USFTracker;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFTurretController : public UObject, public ISFTurretDelegate
{
	GENERATED_BODY()

public:
	USFTurretController(const FObjectInitializer& ObjectInitializer);

	//needed for kismet library
	UWorld* GetWorld() const;

	void Initialize(USFTurretControllerManager* ControllerManager);

	void RegisterTurret(ASFTurretActor* Turret);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsActive() const;

	// returns T/F whether there was any change in state.
	bool SetIsActive(bool bValue);

	//Override API:
	UFUNCTION(BlueprintNativeEvent, Category = "Turret Controller")
	bool OnActivate();

	UFUNCTION(BlueprintNativeEvent, Category = "Turret Controller")
	bool OnDeactivate();

	UFUNCTION(BlueprintCallable)
	bool LeadTarget(ASFTurretActor* Turret, FVector& OutTarget);

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bNeedsTick;

protected:
	UPROPERTY(Transient)
	bool bIsActive;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<ASFTurretActor>> Turrets;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	TArray<ASFTurretActor*> GetTurrets() const;

	UPROPERTY(Transient)
	TWeakObjectPtr<USFTurretControllerManager> ManagerRef;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USFTracker* GetTracker() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APawn* GetPawn() const;

	//SFTurretDelegate
public:
	UFUNCTION()
	virtual bool GetTarget_Implementation(ASFTurretActor* Turret, float DeltaSeconds, FVector& OutVector) override;
};
