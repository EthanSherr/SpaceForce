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
class ASFShipPawn;
class UInputComponent;
class UTimelineComponent;
class UCurveFloat;
class ASFTurretActor;

UCLASS()
class SPACEFORCE_API ASFPilotPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TSubclassOf<class ASFTurretActor>> TurretClasses;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<ASFTurretActor*> Turrets;

	UPROPERTY(EditInstanceOnly, Category = "Debug")
	bool bSpectateDebug;

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

	UPROPERTY(EditAnywhere, Category = "Init")
	ASFShipPawn* InitializeWithShip;

	UPROPERTY(EditAnywhere, Category = "Init", BlueprintReadWrite)
	TArray<FSFRadialMenuOption> OffensiveMenuOptions;

	UPROPERTY(EditAnywhere, Category = "Init", BlueprintReadWrite)
	TArray<FSFRadialMenuOption> DefensiveMenuOptions;


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

//update begin
public:
	virtual void Tick(float DeltaTime) override;

protected:
	void UpdateNextFlightPath();
	void UpdateHandsRoot();
	void UpdateThumbpadAxis();
//update end

//inputs begin
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void OnTriggerDownLeft();
	void OnTriggerDownRight();

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
//inputs end

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HandExtension;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StartedPilotingShip"))
	void ReceiveStartPilotingShip();

//inventory setup
private:
	void SpawnInventory();
	void AddTurret(ASFTurretActor* Turret);
//inventory end
};
