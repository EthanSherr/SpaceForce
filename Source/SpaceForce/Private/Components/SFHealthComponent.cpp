// Fill out your copyright notice in the Description page of Project Settings.


#include "SFHealthComponent.h"

// Sets default values for this component's properties

USFHealthComponent::USFHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = Health = 100.0f;
	MegaDeathThreshold = -10.0f;
}

void USFHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
	if (MegaDeathThreshold > 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("MegaDeathThreshold for %s is %f and must not be > 0.0f"), *GetOwner()->GetName(), MegaDeathThreshold);
		MegaDeathThreshold = -10.0f;
	}
}

float USFHealthComponent::Damage(float DamageAmount)
{
	return ChangeHealth(-DamageAmount);
}

float USFHealthComponent::ChangeHealth(float DeltaHealth)
{
	float UpdatedHealth = Health + DeltaHealth;
	Health = UpdatedHealth;
	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(Health, MaxHealth);
	}
	if (!bActorRemoved)
	{
		if (!DeadBroadcasted && IsDead() && OnDeath.IsBound())
		{
			OnDeath.Broadcast(Health, MaxHealth);
			DeadBroadcasted = true;
		}
		if (!MegaDeadBroadcasted && IsMegaDead() && OnMegaDeath.IsBound())
		{
			OnMegaDeath.Broadcast(Health, MaxHealth);
			MegaDeadBroadcasted = true;
		}
	}

	return Health;
}

bool USFHealthComponent::IsMegaDead()
{
	return Health <= MegaDeathThreshold;
}

bool USFHealthComponent::IsDead()
{
	return Health <= 0;
}

bool USFHealthComponent::IsAlive()
{
	return !IsDead();
}
