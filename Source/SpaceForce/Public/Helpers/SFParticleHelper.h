// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "SFParticleHelper.generated.h"

class UParticleSystemComponent;
class UParticleSystem;
class UWorld;
class UNiagaraComponent;
class UNiagaraSystem;
class UFXSystemComponent;

UCLASS()
class SPACEFORCE_API USFParticleHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Creation")
	static UParticleSystemComponent* CreateCascadeComp(UWorld* World, UParticleSystem* Particles);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Creation")
	static UNiagaraComponent* CreateNiagaraComp(UWorld* World, UNiagaraSystem* Particles);
	
};
