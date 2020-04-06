// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFAimComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USFAimComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class USkeletalMeshComponent* Mesh, float projectileSpeed, FName Barrel = FName("Barrel"), FName Muzzle = FName("Muzzle"));

	UPROPERTY(EditDefaultsOnly)
	float AimSpeed;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetAimSpeed() {
		return AimSpeed;
	}

	// Set target location to give AimComponent a target & valid responses from GetBarrelLookAtLocation
	UFUNCTION(BlueprintCallable)
	void AimAt(FVector target);

	UFUNCTION(BlueprintCallable)
	void AimAtActor(AActor* actor, bool withLead = true);

	//Gives you the desired pitch/yaw for the barrel in component space (used by anim bp)
	UFUNCTION(BlueprintCallable)
	FRotator GetAimCallibration();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE	bool HasTarget() {
		return bWasTargetSet;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE	FVector GetTarget() {
		return Target;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetBarrelLength() {
		return BarrelLength;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetProjectileSpeed() {
		return ProjectileSpeed;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAimingAtTarget(float tolerance = 1);

	UFUNCTION(BlueprintCallable)
	bool Fire();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetMuzzleTransform();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetBarrelTransform();

private:
	bool WasInitialized(bool logError = false);
	//init variables
	class USkeletalMeshComponent* SkeletalMesh;
	const class USkeletalMeshSocket* BarrelSocket;
	const class USkeletalMeshSocket* MuzzleSocket;

	float BarrelLength;
	float ProjectileSpeed;

	//AimAt variables
	bool bWasTargetSet;
	FVector Target;

	AActor* TrackedActor;
	bool bLeadTrackedActor;
};
