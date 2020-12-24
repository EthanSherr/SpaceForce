#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SFSimpleTurretAnimInstance.generated.h"

class USkeletalMeshComponent;
class USkeletalMeshSocket;

UCLASS()
class SPACEFORCE_API USFSimpleTurretAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USFSimpleTurretAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Initialize(FName BarrelNameValue, FName MuzzleNameValue);
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	float GetBarrelLength();

protected:
	bool WasInitialized(bool logError = false);

	//init variables
	UPROPERTY(Transient)
	const USkeletalMeshSocket* BarrelSocket;
	UPROPERTY(Transient)
	const USkeletalMeshSocket* MuzzleSocket;

	UPROPERTY(Transient)
	FTransform InitialBarrelTransformCS;

	UPROPERTY(Transient)
	float BarrelLength;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Output")
	float Pitch;

	UPROPERTY(Transient, BlueprintReadonly, Category = "Animation Output")
	float Yaw;
};
