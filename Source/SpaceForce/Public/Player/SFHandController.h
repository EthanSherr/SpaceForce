// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "SFHandState.h"
#include "SFHandController.generated.h"

class USphereComponent;
class ASFFlightPath;
class ASFPilotPawn;
class ASFShipPawn;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFHandController : public UMotionControllerComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* ShipScanner;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* PathScanner;

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

private:
	UPROPERTY(BlueprintSetter = SetHandState, BlueprintGetter = GetHandState)
	TEnumAsByte<EHandState> HandState;
};
