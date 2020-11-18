// Fill out your copyright notice in the Description page of Project Settings.


#include "SFParticleHelper.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"

UParticleSystemComponent* USFParticleHelper::CreateCascadeComp(UWorld* World, UParticleSystem* System)
{
	UParticleSystemComponent* PS = NewObject<UParticleSystemComponent>(World);
	PS->bAutoActivate = true;
	PS->bAutoDestroy = true;
	PS->RegisterComponentWithWorld(World);
	PS->SetTemplate(System);
	return PS;
}

UNiagaraComponent* USFParticleHelper::CreateNiagaraComp(UWorld* World, UNiagaraSystem* Particles)
{
	UNiagaraComponent* NS = NewObject<UNiagaraComponent>(World);
	NS->SetAsset(Particles);
	NS->RegisterComponentWithWorld(World);
	return NS;
}