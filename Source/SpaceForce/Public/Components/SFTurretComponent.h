// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SFTurretComponent.generated.h"

class USkeletalMeshComponent;
class USkeletalMeshSocket;

USTRUCT(Blueprintable)
struct SPACEFORCE_API FAimCallibration
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FRotator From;

	UPROPERTY(BlueprintReadWrite)
	FRotator To;

	FAimCallibration() : From(FRotator::ZeroRotator), To(FRotator::ZeroRotator) {}
	FAimCallibration(FRotator from, FRotator to) : From(from), To(to) {}
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;

protected:
	virtual void BeginPlay() override;

	// grabs sockets from skeletalmesh
	UFUNCTION(BlueprintCallable)
	void Initialize();

public:
	UFUNCTION(BlueprintCallable)
	void Debug();

	UPROPERTY(EditAnywhere)
	bool bDebug;

	void DebugTransform(FTransform transform, float time = 0.0f);

public: // turret interface

	// Set target location to give AimComponent a target & valid responses from GetBarrelLookAtLocation
	UFUNCTION(BlueprintCallable)
	void AimAt(FVector target);

	UFUNCTION(BlueprintCallable)
	void AimAtActor(AActor* actor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAimingAtTarget(float tolerance = 1);

	UFUNCTION(BlueprintCallable)
	FAimCallibration GetAimCallibration();

	UFUNCTION(BlueprintCallable)
		FAimCallibration GetAimCallibrationOld();

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

	AActor* TrackedActor;

	bool UpdateTargetFromTrackedActor();

public:
	UPROPERTY(BlueprintReadonly)
	FVector Target;
};
