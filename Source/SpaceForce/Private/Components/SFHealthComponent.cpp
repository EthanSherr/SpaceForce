// Fill out your copyright notice in the Description page of Project Settings.


#include "SFHealthComponent.h"

// Sets default values for this component's properties

USFHealthComponent::USFHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = Health = 100.0f;
}

float USFHealthComponent::ChangeHealth(float DeltaHealth)
{
	bool AlreadyDead = IsDead();
	float UpdatedHealth = Health + DeltaHealth;
	Health = UpdatedHealth;
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(Health, MaxHealth);
	}
	if (!AlreadyDead && IsDead() && OnDeath.IsBound())
	{
		OnDeath.Broadcast(Health, MaxHealth);
	}

	return Health;
}

bool USFHealthComponent::IsDead()
{
	return Health <= 0;
}

bool USFHealthComponent::IsAlive()
{
	return !IsDead();
}
