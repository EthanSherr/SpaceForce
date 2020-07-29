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
	UPROPERTY(EditDefaultsOnly, Category = "Aim Initialization")
	bool bLeadTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim Initialization")
	float ProjectileSpeed;

public:
	UPROPERTY(EditDefaultsOnly, Category = "SKM Initialization")
	FName BarrelName;

	UPROPERTY(EditDefaultsOnly, Category = "SKM Initialization")
	FName MuzzleName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, BlueprintGetter = GetAimSpeed, Category = "SKM Initialization")
	float AimSpeed;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMesh;

protected:
	virtual void BeginPlay() override;

	// grabs sockets from skeletalmesh
	UFUNCTION(BlueprintCallable)
	void Initialize();

public:
	UFUNCTION(BlueprintCallable)
	void Debug();

public: // turret interface

	// Set target location to give AimComponent a target & valid responses from GetBarrelLookAtLocation
	UFUNCTION(BlueprintCallable)
	void AimAt(FVector target);

	UFUNCTION(BlueprintCallable)
	void AimAtActor(AActor* actor, bool withLead = true);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAimingAtTarget(float tolerance = 1);

	UFUNCTION(BlueprintCallable)
	FAimCallibration GetAimCallibration();

public: // getters
	//Gives you the desired pitch/yaw for the barrel in component space (used by anim bp)
	UFUNCTION(BlueprintCallable, BlueprintGetter)
	FORCEINLINE float GetAimSpeed() { return AimSpeed; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE	bool HasTarget() { return bWasTargetSet; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE	FVector GetTarget() { return Target; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetBarrelLength() { return BarrelLength; }

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
