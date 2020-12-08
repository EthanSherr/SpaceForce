// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFDeflectable.generated.h"

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFDeflectable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFDeflectable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Deflectable")
	bool ShouldDeflectProjectile(const FHitResult& Hit);
};
