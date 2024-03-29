// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFHandState.h"
#include "Components/TimelineComponent.h"
#include "../UI/SFRadialMenuOption.h"
#include "SFPilotPawn.generated.h"

class USceneComponent;
class USFHandController;
class UCameraComponent;
class USFSplineMovementComponent;
class USteamVRChaperoneComponent;
class ASFPlayerShip;
class UInputComponent;
class UTimelineComponent;
class UCurveFloat;
class ASFTurretActor;
class USFRadialMenuComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShipPilotBegan);

UCLASS()
class SPACEFORCE_API ASFPilotPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Debug")
	bool bSpectateDebug;

	UPROPERTY(Category = "Barrel Role", EditAnywhere)
	float BarrelRoleThreshold;

	UPROPERTY(Category = "Barrel Role", EditAnywhere)
	float BarrelRoleTimeThreshold;

	UPROPERTY(BlueprintReadOnly)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	ASFPlayerShip* InitializeWithShip;

	UPROPERTY(EditAnywhere, Category = "Init", BlueprintReadWrite)
	TArray<FSFRadialMenuOption> OffensiveMenuOptions;

	UPROPERTY(EditAnywhere, Category = "Init", BlueprintReadWrite)
	TArray<FSFRadialMenuOption> DefensiveMenuOptions;


	// Gets set once ship is being piloted, one hand is now driving.
	UPROPERTY(BlueprintReadOnly)
	ASFPlayerShip* Ship;
public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
	USFHandController* GetHandInState(TEnumAsByte<EHandState> HandState);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	USFHandController* GetOtherHand(USFHandController* Hand);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
	void OnTrigger(USFHandController* Hand, bool bIsPressed);

	UFUNCTION(BlueprintCallable)
	void StartPilotingShip(USFHandController* Hand, ASFPlayerShip* NewShip);

	UPROPERTY(BlueprintAssignable)
	FOnShipPilotBegan OnShipPilotBegan;

	UFUNCTION(BlueprintCallable)
	void ActivateTurret(int Index);

public:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

//update begin
public:
	virtual void Tick(float DeltaTime) override;

protected:
	void UpdateNextFlightPath();
	void UpdateHandsRoot();
	void UpdateThumbpadAxis();
	void DetectHandRole(float DeltaTime);
//update end

//inputs begin
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void OnTriggerDownLeft();
	void OnTriggerDownRight();
	void OnTriggerUpLeft();
	void OnTriggerUpRight();

	void OnLeftGripDown();
	void OnLeftGripUp();
	void OnRightGripDown();
	void OnRightGripUp();

	void OnLeftTouchDown();
	void OnLeftTouchUp();
	void OnRightTouchDown();
	void OnRightTouchUp();

	void OnRightClickDown();
	void OnLeftClickDown();

	void OnGrip(USFHandController* Hand, bool bIsPressed);
	void OnThumbpadTouch(USFHandController* Hand, bool bIsPressed);
	void OnThumbpadClick(USFHandController* Hand, bool bIsPressed);

	UFUNCTION()
	void MenuItemSelected(USFRadialMenuComponent* Menu, FSFRadialMenuOption Option, int Index);

//inputs end

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HandExtension;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StartedPilotingShip"))
	void ReceiveStartPilotingShip();

// barrel role detection
private:
	UPROPERTY(Transient)
	FQuat LastDrivingHandRotation;

protected:
	UFUNCTION()
	void RegisterBarrelRole(float Direction);

	UPROPERTY(Transient, BlueprintReadWrite)
	bool bIsDoingBarrelRole;

	UPROPERTY(Transient, BlueprintReadOnly)
	float BarrelRoleInput;

	UPROPERTY(Transient)
	FTimerHandle RoleThresholdTimer;

	UFUNCTION()
	void DoBarrelRoleTimerHandler();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveDoABarrelRole(float Direction);
};
