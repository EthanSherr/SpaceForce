// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SFDustComponent.generated.h"

class UArrowComponent;
class UFXSystemComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFDustComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	USFDustComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	bool bDebug;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DustFX;

	UPROPERTY(EditInstanceOnly)
	UArrowComponent* ArrowComponent;

	UPROPERTY()
	USceneComponent* Ground;

	UPROPERTY(EditAnywhere)
	float TraceDistance;

protected:

	UFUNCTION()
	void SpawnNewDustEffect(UNiagaraSystem* System);

	UPROPERTY(Transient)
	UNiagaraComponent* GroundEffect;

	UFUNCTION()
	void SetGroundDistance(float Distance);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
