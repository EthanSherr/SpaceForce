// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SFSpringFlightMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct SPACEFORCE_API FSpringConfig {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Stiffness;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Damping;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxExtension;

	FSpringConfig() : Stiffness(0.0f), Damping(0.0f), MaxExtension(0.0f) {}

	FSpringConfig(float stiffness, float damping, float maxExtension) : Stiffness(stiffness), Damping(damping), MaxExtension(maxExtension) {}

	static FSpringConfig FromCriticalDampingAndMaxSpeed(float Stiffness, float CriticalDamping, float MaxSpeed, float Mass) {
		float Damping = CriticalDamping * 2 * FMath::Sqrt(Stiffness * Mass);
		float MaxExtension = Damping * MaxSpeed / Stiffness;
		return FSpringConfig(Stiffness, Damping, MaxExtension);
	}

	FORCEINLINE FString ToString() const {
		return FString::Printf(TEXT("Stiffness=%3.3f Damping=%3.3f MaxExtension=%3.3f"), Stiffness, Damping, MaxExtension);
	}
};

class AActor;

UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), ShowCategories = (Velocity))
class SPACEFORCE_API USFSpringFlightMovementComponent : public UPawnMovementComponent
{
	GENERATED_UCLASS_BODY()

public:


	inline float GetMaxSpeed() const override
	{
		//UE_LOG(LogTemp, WARNING, TEXT("Returning GetMaxSpeed %f", LinearMaxSpeed))
		return LinearMaxSpeed;
	}

	UPROPERTY(EditAnywhere, Category = "Movement")
	float LinearStiffness;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float LinearCriticalDamping;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float LinearMaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AngularStiffnessPrimary;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AngularDampingPrimary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AngularStiffnessSecondary;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AngularDampingSecondary;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bMaintainMaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bUseTargetOrientation;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugTorque;

	UPROPERTY(EditAnywhere, Category = "Movement")
	AActor* InitialTarget;

// Begin target
	UPROPERTY(BlueprintGetter = GetTarget, BlueprintSetter = SetTarget)
	FVector Target;

	UPROPERTY(BlueprintGetter = GetTargetComponent, BlueprintSetter = SetTargetComponent)
	USceneComponent* TargetComponent;

	UFUNCTION(BlueprintGetter)
	FVector GetTarget();

	UFUNCTION(BlueprintSetter)
	void SetTarget(FVector value);

	UFUNCTION(BlueprintGetter)
	USceneComponent* GetTargetComponent();

	UFUNCTION(BlueprintSetter)
	void SetTargetComponent(USceneComponent* NewComponent);

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float NewLinearMaxSpeed);

	UFUNCTION(BlueprintCallable)
	void SetLinearStiffness(float NewLinearStiffness);

	UFUNCTION(BlueprintCallable)
	void RecomputeSpringConfig();

	UFUNCTION(BlueprintCallable)
	void ClearTarget();

	UPROPERTY(EditAnywhere)
	bool bCalculateVelocityOfTarget = true;

private:
	UPROPERTY()
	bool bHasTarget;

	void UpdateTarget(float DeltaTime);
// End target
	
protected:

	void AddInputVector(FVector WorldVector, bool bForce = false) override;

	UPROPERTY(Transient)
	FSpringConfig SpringConfig;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Spin")
	void ReceiveIsSpinning(bool bValue);

	UPROPERTY(Transient, BlueprintReadonly)
	bool bIsSpinning;

private:
	FVector CalculateForces(FVector P2, FVector P2Velocity);

	FVector CalculateSpringDampingForces(FVector CurrentLocation, FVector TargetLocation, FVector CurrentVelocity, FVector TargetVelocity, float Ks, float Kd, float MaxExtension);

	FVector CalculateSpringDampingTorque(FVector CurrentOrientation, FVector TargetOrientation, FVector AngularVelocity, float Ks, float Kd);

	bool IsValid(bool logError = false);

	class UPrimitiveComponent* GetUpdatedPrimitiveComp();

	FVector DefaultForward = FVector::ForwardVector;

	FVector TargetVelocity = FVector::ZeroVector;

	FVector LastTargetPosition;
};
