// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SFBehaviorTreePawn.h"
#include "SFFlyingAIPawn.generated.h"

class USFSpringFlightMovementComponent;
class USFHealthComponent;

USTRUCT(BlueprintType)
struct FSFAIFlightParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Infront")
	float MaxSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Infront")
	float MinSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Stay Infront")
	bool bSpeedRelativeToShip;
};

UCLASS()
class SPACEFORCE_API ASFFlyingAIPawn : public ASFBehaviorTreePawn
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ShipStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFSpringFlightMovementComponent* FlightMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSFAIFlightParams FlightParams;
};
