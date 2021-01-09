// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthEventDelegate, class USFHealthComponent*, HealthComponent, float, Damage);

UCLASS(ClassGroup = "Custom", meta = (BlueprintSpawnableComponent))
class SPACEFORCE_API USFHealthComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	
	UPROPERTY(BlueprintReadonly)
	class AController* LastEventInstigator;
	UPROPERTY(BlueprintReadonly)
	class AActor* LastDamageCauser;

	UFUNCTION(BlueprintCallable)
	float Damage(float DamageAmount);

	UFUNCTION(BlueprintCallable)
	float ChangeHealth(float DeltaHealth, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

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

	UPROPERTY(BlueprintReadOnly, Transient)
	float Health;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bGodMode;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MegaDeathThreshold;

	UFUNCTION(BlueprintCallable)
	void UnbindAllDeathEvents(UObject* Target);

protected:
	virtual void InitializeComponent() override;
private:
	bool DeadBroadcasted;
	bool MegaDeadBroadcasted;
};
