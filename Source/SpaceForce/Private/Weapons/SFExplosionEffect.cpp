// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SFExplosionEffect.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

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

void ASFExplosionEffect::BeginPlay()
{
	Super::BeginPlay();
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
}