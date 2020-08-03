// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthEventDelegate, float, NewHealth, float, MaxHealth);

UCLASS(ClassGroup = "Custom", meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFHealthComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	

	UFUNCTION(BlueprintCallable)
	float Damage(float DamageAmount);

	UFUNCTION(BlueprintCallable)
	float ChangeHealth(float DeltaHealth);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsMegaDead();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsDead();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool IsAlive();

	UPROPERTY(BlueprintAssignable)
	FHealthEventDelegate OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FHealthEventDelegate OnDeath;

	UPROPERTY(BlueprintAssignable)
	FHealthEventDelegate OnMegaDeath;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Health;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MegaDeathThreshold;

	UPROPERTY(BlueprintReadWrite)
	bool bActorRemoved;

protected:
	virtual void InitializeComponent() override;
private:
	bool DeadBroadcasted;
	bool MegaDeadBroadcasted;
};
