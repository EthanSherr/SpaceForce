// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFExplosionEffect.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SPACEFORCE_API ASFExplosionEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFExplosionEffect(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	virtual void PreInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	UNiagaraSystem* ExplosionFX;

protected:
	UPROPERTY(BlueprintReadonly)
	UNiagaraComponent* NiagaraComponent;

};
