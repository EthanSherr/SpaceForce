// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFHandState.h"
#include "SFPilotPawn.generated.h"

class USceneComponent;
class USFHandController;
class UCameraComponent;
class USFSplineMovementComponent;
class USteamVRChaperoneComponent;
class ASFShipPawn;
class UInputComponent;

UCLASS()
class SPACEFORCE_API ASFPilotPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* HandsRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USFHandController* LeftHand;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USFHandController* RightHand;

	UPROPERTY(EditDefaultsOnly)
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	void OnTriggerDownLeft();
	void OnTriggerDownRight();

};
