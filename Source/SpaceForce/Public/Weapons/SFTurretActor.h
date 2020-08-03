// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFTurretActor.generated.h"

class USFTurretComponent;
class USkeletalMeshComponent;
class ASFProjectile;
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
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USFTurretComponent* TurretComponent;

	virtual void PostInitializeComponents() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SocketName;

	UFUNCTION(BlueprintCallable)
	void AimAt(FVector Target);

	UFUNCTION(BlueprintCallable)
	void AimAtActor(AActor* TargetActor);

	UFUNCTION(BlueprintNativeEvent)
	void TriggerAction(bool bIsPressed);

	UFUNCTION(BlueprintPure)
	float GetTriggetAxis();

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FTransform& Transform);

	// UI
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	UMaterialInterface* MaterialIcon;
};
