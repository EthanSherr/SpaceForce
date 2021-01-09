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
	Health = MaxHealth;
	if (MegaDeathThreshold > 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("MegaDeathThreshold for %s is %f and must not be > 0.0f"), *GetOwner()->GetName(), MegaDeathThreshold);
		MegaDeathThreshold = -10.0f;
	}
}

float USFHealthComponent::Damage(float DamageAmount)
{
	return ChangeHealth(-DamageAmount, FDamageEvent(), NULL, NULL);
}

float USFHealthComponent::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	return ChangeHealth(-Damage, DamageEvent, EventInstigator, DamageCauser);
}

float USFHealthComponent::ChangeHealth(float DeltaHealth, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (bGodMode) return 0.0f;
	LastEventInstigator = EventInstigator;
	LastDamageCauser = DamageCauser;

	float UpdatedHealth = Health + DeltaHealth;
	Health = UpdatedHealth;

	if (OnHealthChanged.IsBound())
	{
		OnHealthChanged.Broadcast(this, DeltaHealth);
	}
	if (!DeadBroadcasted && IsDead() && OnDeath.IsBound())
	{
		OnDeath.Broadcast(this, DeltaHealth);
		DeadBroadcasted = true;
	}
	if (!MegaDeadBroadcasted && IsMegaDead() && OnMegaDeath.IsBound())
	{
		OnMegaDeath.Broadcast(this, DeltaHealth);
		MegaDeadBroadcasted = true;
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

void USFHealthComponent::UnbindAllDeathEvents(UObject* Target)
{
	OnMegaDeath.RemoveAll(Target);
	OnDeath.RemoveAll(Target);
}
