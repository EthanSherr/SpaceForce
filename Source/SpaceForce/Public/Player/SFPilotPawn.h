// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFHandState.h"
#include "Components/TimelineComponent.h"
#include "SFPilotPawn.generated.h"

class USceneComponent;
class USFHandController;
class UCameraComponent;
class USFSplineMovementComponent;
class USteamVRChaperoneComponent;
class ASFShipPawn;
class UInputComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class SPACEFORCE_API ASFPilotPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditInstanceOnly)
	bool bSpectateDebug;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* HandsRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USFHandController* LeftHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USFHandController* RightHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFSplineMovementComponent* SplineMovement;

	UPROPERTY(EditDefaultsOnly)
	USteamVRChaperoneComponent* VRChaperone;

	UPROPERTY(EditAnywhere)
	ASFShipPawn* InitializeWithShip;

	// Gets set once ship is being piloted, one hand is now driving.
	UPROPERTY(BlueprintReadOnly)
	ASFShipPawn* Ship;
public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
	USFHandController* GetHandInState(TEnumAsByte<EHandState> HandState);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	USFHandController* GetOtherHand(USFHandController* Hand);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
	void OnTriggerDown(USFHandController* Hand);

	UFUNCTION(BlueprintCallable)
	void StartPilotingShip(USFHandController* Hand, ASFShipPawn* NewShip);

public:
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void OnTriggerDownLeft();
	void OnTriggerDownRight();

	void OnLeftGripDown();
	void OnLeftGripUp();
	void OnRightGripDown();
	void OnRightGripUp();

	void OnGrip(USFHandController* Hand, bool bIsPressed);

	void TrySetIsBoosting(bool bNewIsBoosting);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HandExtension;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StartedPilotingShip"))
	void ReceiveStartPilotingShip();

// Boost
protected:

	UFUNCTION()
	void SetSpeedBoostDelta(float NewSpeedBoostDelta);

	UFUNCTION()
	void SetLinearStiffnessBoostDelta(float NewLinearStiffnessBoostDelta);

	UTimelineComponent* BoostTimeline;

	UPROPERTY(Transient)
	float SpeedBoostSpeedDelta;

	UPROPERTY(Transient)
	float LinearStiffnessBoostDelta;

	UPROPERTY(Transient)
	bool bIsBoosting;

	FOnTimelineFloat SpeedBoostTimelineUpdateDelegate;
	FOnTimelineFloat LinearStiffnessBoostTimelineUpdateDelegate;
	FOnTimelineEvent BoostTimelineFinishedDelegate;

	UFUNCTION()
	void SpeedBoostTimelineUpdate(float Value);

	UFUNCTION()
	void LinearStiffnessBoostTimelineUpdate(float Value);

	UFUNCTION()
	void BoostTimelineFinished();
public:

	UPROPERTY(EditAnywhere, Category = "Boost")
	UCurveFloat* SpeedBoostCurve;

	UPROPERTY(EditAnywhere, Category = "Boost")
	UCurveFloat* LinearStiffnessBoostCurve;

	UPROPERTY(EditInstanceOnly, Category = "Boost")
	float SpeedBoostDecay;

	UPROPERTY(EditInstanceOnly, Category = "Boost")
	float LinearStiffnessBoostDecay;

	UPROPERTY(BlueprintReadOnly, Category = "Boost|Energy")
	float BoosterEnergy;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Boost|Energy")
	float MaximumBoosterEnergy;

	UPROPERTY(EditInstanceOnly, Category = "Boost|Energy")
	float MinimumEnergyToStartBoost;

	UPROPERTY(EditInstanceOnly, Category = "Boost|Energy")
	float BoostEnergyDecayRate;

	UPROPERTY(EditInstanceOnly, Category = "Boost|Energy")
		float BoostEnergyRegenRate;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnBoostStart(bool& bSuccess);

	UFUNCTION(BlueprintImplementableEvent)
	void OnBoostStop();
//End Boost
};
