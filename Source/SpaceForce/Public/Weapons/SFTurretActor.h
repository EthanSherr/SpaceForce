// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFTurretActor.generated.h"

class USFTurretComponent;
class USkeletalMeshComponent;
class ASFProjectile;
class ASFAimVisualization;
class USoundBase;

UCLASS()
class SPACEFORCE_API ASFTurretActor : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	TSubclassOf<ASFProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	float ProjectileSpeedOverride;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	USoundBase* ActivationSound;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	TSubclassOf<ASFAimVisualization> AimVisualizationTemplate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USFTurretComponent* TurretComponent;

protected:
	UPROPERTY(Transient, BlueprintReadonly)
	ASFAimVisualization* AimVisualization;

public:

	virtual void PostInitializeComponents() override;

	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SocketName;

	UFUNCTION(BlueprintCallable)
	void AimAt(FVector Target);

	UFUNCTION(BlueprintCallable)
	void AimAtComponent(USceneComponent* SceneComponent);

	UFUNCTION(BlueprintNativeEvent)
	void TriggerAction(bool bIsPressed);

	UFUNCTION(BlueprintPure)
	float GetTriggetAxis();

	UFUNCTION(BlueprintCallable)
	ASFProjectile* SpawnProjectile(const FTransform& Transform);

	UFUNCTION()
	void SetActivated(bool bValue);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveActivated(bool bValue);

	// UI
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	UMaterialInterface* MaterialIcon;
};
