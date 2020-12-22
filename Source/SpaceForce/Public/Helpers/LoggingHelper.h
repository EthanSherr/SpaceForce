// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LoggingHelper.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API ULoggingHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MyCategory")
    static FString GetNameOrNull(const UObject* Object);

};
