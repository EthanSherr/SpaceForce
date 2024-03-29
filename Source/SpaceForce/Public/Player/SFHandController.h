// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "SFHandState.h"
#include "SFHandController.generated.h"

class ASFFlightPath;
class ASFPilotPawn;
class ASFPlayerShip;
class USphereComponent;
class USFRadialMenuComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFHandController : public UMotionControllerComponent
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION()
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	USphereComponent* ShipScanner;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* PathScanner;

	UPROPERTY(BlueprintReadonly, EditDefaultsOnly)
	USceneComponent* ShipTargetComponent;

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
	ASFPlayerShip* GetOverlappingShip();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFPilotPawn* GetPilot();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFPlayerShip* GetShip();

	UFUNCTION()
	bool RecievesInput();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EControllerHand HandTypeFromMotionSource();

	UFUNCTION()
	void OnTrigger(bool& OutbCapturesInput, const bool& bIsPressed);

	UPROPERTY(BlueprintReadonly, EditInstanceOnly)
	USFRadialMenuComponent* RadialMenuComponent;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FocusInteractables();

private:
	UPROPERTY(BlueprintSetter = SetHandState, BlueprintGetter = GetHandState)
	TEnumAsByte<EHandState> HandState;

	TWeakObjectPtr<AActor> FocusedActor;
};
