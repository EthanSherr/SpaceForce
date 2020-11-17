// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SFDustComponent.generated.h"

class UArrowComponent;
class UParticleSystemComponent;
class UParticleSystem;

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFDustComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	USFDustComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	bool bDebug;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DustFX;

	UPROPERTY(Transient)
	bool SystemPaused;

	UPROPERTY(EditInstanceOnly)
	UArrowComponent* ArrowComponent;

	UPROPERTY()
	USceneComponent* Ground;

	UPROPERTY(EditAnywhere)
	float TraceDistance;

	UPROPERTY(BlueprintReadonly)
	float Intensity;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnNewDustEffect();

	UPROPERTY(Transient)
	UParticleSystemComponent* Particles;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
