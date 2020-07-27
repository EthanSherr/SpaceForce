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

UCLASS()
class SPACEFORCE_API ASFPilotPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
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

	UPROPERTY(EditAnywhere, Category = "Init")
	TArray<FSFRadialMenuOption> OffensiveMenuOptions;

	UPROPERTY(EditAnywhere, Category = "Init")
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HandExtension;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "StartedPilotingShip"))
	void ReceiveStartPilotingShip();
};
