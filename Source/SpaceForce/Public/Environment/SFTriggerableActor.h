// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFTriggerData.h"
#include "SFTriggerableActor.generated.h"

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFTriggerableActor : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFTriggerableActor
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "RespondToTrigger")
	void RespondToTrigger(class AActor* Source, const FSFTriggerData& Data);
};