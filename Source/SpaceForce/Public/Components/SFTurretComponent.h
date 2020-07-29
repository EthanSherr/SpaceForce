// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SFAimComponent.h"
#include "SFTurretComponent.generated.h"

class USkeletalMeshComponent;
class USkeletalMeshSocket;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USFTurretComponent : public USceneComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Init")
	FName BarrelName;

	UPROPERTY(EditInstanceOnly, Category = "Init")
	FName MuzzleName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Init")
	float AimSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Init")
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void Debug();

	UFUNCTION(BlueprintCallable)
	void Initialize();

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
	FAimCallibration GetAimCallibration();

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
	const USkeletalMeshSocket* BarrelSocket;
	const USkeletalMeshSocket* MuzzleSocket;
	FTransform InitialBarrelTransformCS;
	float BarrelLength;

	//AimAt variables
	bool bWasTargetSet;
	FVector Target;

	AActor* TrackedActor;
	bool bLeadTrackedActor;

	bool UpdateTargetFromTrackedActor();
};
