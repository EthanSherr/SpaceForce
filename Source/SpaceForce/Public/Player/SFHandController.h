// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "SFHandState.h"
#include "SFHandController.generated.h"

class ASFFlightPath;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFHandController : public UMotionControllerComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* ShipScanner;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* PathScanner;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EHandState> HandState;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFFlightPath* GetNearestFlightPath();

	UFUNCTION(BlueprintCallable)
	void BeginDriving(AActor* Ship);
};
