// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "SFHandState.h"
#include "SFHandController.generated.h"

class ASFFlightPath;
class ASFPilotPawn;
class ASFShipPawn;
class USphereComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFHandController : public UMotionControllerComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* ShipScanner;

	UPROPERTY(EditAnywhere)
	USphereComponent* PathScanner;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bScanForInteractables;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float ScanDistance;

	UFUNCTION(BlueprintSetter)
	void SetHandState(TEnumAsByte<EHandState> NewState);

	UFUNCTION(BlueprintGetter)
	TEnumAsByte<EHandState> GetHandState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFFlightPath* GetNearestFlightPath(ASFFlightPath* PathToIgnore);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFShipPawn* GetOverlappingShip();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFPilotPawn* GetPilot();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFShipPawn* GetShip();

	UFUNCTION()
	bool RecievesInput();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EControllerHand HandTypeFromMotionSource();

	UFUNCTION()
	void OnTriggerDown(bool& OutbCapturesInput);
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FocusInteractables();

private:
	UPROPERTY(BlueprintSetter = SetHandState, BlueprintGetter = GetHandState)
	TEnumAsByte<EHandState> HandState;

	TWeakObjectPtr<AActor> FocusedActor;
};
