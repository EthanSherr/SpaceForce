// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/SFTurretDelegate.h"
#include "SFTurretActor.generated.h"

class ASFProjectile;
class ASFAimVisualization;
class USoundBase;
class USkeletalMeshComponent;

UCLASS()
class SPACEFORCE_API ASFTurretActor : public AActor, public ISFTurretDelegate
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName SocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	TSubclassOf<ASFProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Initialization")
	float ProjectileSpeedOverride;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	USoundBase* ActivationSound;

	UPROPERTY(EditDefaultsOnly, Category = "Initialization")
	TSubclassOf<ASFAimVisualization> AimVisualizationTemplate;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly, Category = "Initialization")
	USkeletalMeshComponent* TurretComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FName MuzzleName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FName BarrelName;

// TurretDelegate
	UFUNCTION(BlueprintCallable)
	virtual bool GetTarget_Implementation(FVector& OutTarget) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBarrelLength();

protected:
	UPROPERTY(Transient, BlueprintReadonly)
	ASFAimVisualization* AimVisualization;

public:

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetBarrelTransform() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetMuzzleTransform() const;

	UFUNCTION(BlueprintNativeEvent)
	void TriggerAction(bool bIsPressed);

	//REMOVE?
	UFUNCTION(BlueprintPure)
	float GetTriggetAxis();

	UFUNCTION(BlueprintCallable)
	ASFProjectile* SpawnProjectile(const FTransform& Transform);

	UFUNCTION()
	void SetActivated(bool bValue);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveActivated(bool bValue);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetProjectileSpeed() const;

	// UI
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	UMaterialInterface* MaterialIcon;

private:
	UPROPERTY(Transient)
	bool bActivated;

	UPROPERTY(Transient)
	float ProjectileSpeed;
};
