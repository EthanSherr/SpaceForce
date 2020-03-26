// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFPlayerTriggerResponder.generated.h"

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFPlayerTriggerResponder : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFPlayerTriggerResponder
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PlayerTriggerResponder")
	void PlayerEnteredRegion();
};
