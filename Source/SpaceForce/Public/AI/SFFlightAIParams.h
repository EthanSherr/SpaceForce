// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFFlightAIParams.generated.h"

class UCurveFloat;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFFlightAIParams : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="StayInfront")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "StayInfront")
	float MinSpeed;

	UPROPERTY(EditAnywhere, Category = "StayInfront")
	bool SpeedRelativeToPlayer;

	UPROPERTY(EditAnywhere, Category = "StayInfront")
	float MinDistance;

	UPROPERTY(EditAnywhere, Category = "StayInfront")
	float MaxDistance;

	UPROPERTY(EditAnywhere, Category = "StayInfront")
	UCurveFloat* BoostCurve;
};
