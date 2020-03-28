// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, float, NewHealth);

UCLASS(ClassGroup = "Custom", meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFHealthComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	

	UFUNCTION(BlueprintCallable)
	float ChangeHealth(float DeltaHealth);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsAlive();

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Health;
};
