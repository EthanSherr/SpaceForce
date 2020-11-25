// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SFExplosionEffect.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

ASFExplosionEffect::ASFExplosionEffect(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	NiagaraComponent = ObjectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, FName("NiagaraComponent"));
	NiagaraComponent->bAutoActivate = true;
}

void ASFExplosionEffect::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	if (ExplosionFX)
	{
		NiagaraComponent->SetAsset(ExplosionFX);
	}
}
