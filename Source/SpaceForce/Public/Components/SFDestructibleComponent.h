// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleComponent.h"
#include "SFDestructibleComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API USFDestructibleComponent : public UDestructibleComponent
{
	GENERATED_BODY()
public:
	USFDestructibleComponent(const FObjectInitializer& ObjectInitializer);
	virtual void ReceiveComponentDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
};
