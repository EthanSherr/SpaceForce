// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFTurretDelegate.generated.h"

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFTurretDelegate : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFTurretDelegate
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TurretDelegate")
	bool Fire(class USFTurretComponent* TurretComponent);
};