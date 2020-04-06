// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SFAIController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API ASFAIController : public AAIController
{
    GENERATED_UCLASS_BODY()

public:

    UFUNCTION(BlueprintCallable)
    void AttackActor(AActor* actor);

};
