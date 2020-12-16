// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SFShipPawn.h"
#include "SFPlayerShip.generated.h"

class USFBoosterManagerComponent;

UCLASS()
class SPACEFORCE_API ASFPlayerShip : public ASFShipPawn
{
	GENERATED_BODY()
public:


public:
    ASFPlayerShip(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    USFBoosterManagerComponent* BoosterManagerComponent;

    UFUNCTION(BlueprintCallable)
    void TrySetIsBoosting(bool bNewIsBoosting);
};
