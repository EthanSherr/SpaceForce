// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFAimComponent.generated.h"
USTRUCT(Blueprintable)
struct SPACEFORCE_API FAimCallibration
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FRotator From;

	UPROPERTY(BlueprintReadWrite)
	FRotator To;

	FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("from: (%s) to: (%s)"), *From.ToString(), *To.ToString());
	}

	FORCEINLINE FAimCallibration() : From(FRotator::ZeroRotator), To(FRotator::ZeroRotator) {};
	FORCEINLINE FAimCallibration(FRotator from, FRotator to) : From(from), To(to) {};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USFAimComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize(class USkeletalMeshComponent* Mesh, FName Barrel = FName("Barrel"), FName Muzzle = FName("Muzzle"));

	// Set target location to give AimComponent a target & valid responses from GetBarrelLookAtLocation
	UFUNCTION(BlueprintCallable)
	void AimAt(FVector target);

	//Gives you the desired pitch/yaw for the barrel in component space (used by anim bp)
	UFUNCTION(BlueprintCallable, BlueprintPure)
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

private:
	bool WasInitialized(bool logError = false);
	//init variables
	class USkeletalMeshComponent* SkeletalMesh;
	const class USkeletalMeshSocket* BarrelSocket;
	const class USkeletalMeshSocket* MuzzleSocket;

	float BarrelLength;

	//AimAt variables
	bool bWasTargetSet;
	FVector Target;
};
