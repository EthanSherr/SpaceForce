// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFShipPawn.generated.h"

class USFSplineMovementComponent;
class USFSpringFlightMovementComponent;
class ASFPilotPawn;

UCLASS()
class SPACEFORCE_API ASFShipPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ShipStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFSpringFlightMovementComponent* FlightMovement;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFPilotPawn* GetOwnerPilot();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USFSplineMovementComponent* GetAssociatedSplineMovementComponent();

private:
	UPROPERTY()
	USFSplineMovementComponent* AssociatedSplineMovementComponent;
};
