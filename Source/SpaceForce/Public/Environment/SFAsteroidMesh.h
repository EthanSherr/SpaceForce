// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFAsteroidMesh.generated.h"

class UArrowComponent;
class UStaticMeshComponent;
class UAsteroidVisComponent;

UCLASS()
class SPACEFORCE_API ASFAsteroidMesh : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* AsteroidMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector LinearVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector AngularVelocity;

	UPROPERTY(EditAnywhere)
	UArrowComponent* DirectionTarget;

	UPROPERTY(EditAnywhere)
	float DirectionTargetMagnitudeScale = 100.0f;

	UPROPERTY(EditAnywhere)
	bool bActivateOnBegin;

	UFUNCTION(BlueprintCallable)
	void SetActive(bool InActive);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Debug")
	uint8 bKeepDebugDisplayOn : 1;

	UPROPERTY(EditAnywhere, Category = "Debug")
	int DebugTotalSteps;

	UPROPERTY(EditAnywhere, Category = "Debug")
	int DebugAtTime;

	UPROPERTY(EditAnywhere, Category = "Debug")
	float SegmentSizeInSeconds;

	UPROPERTY()
	UAsteroidVisComponent* VisComponent;
#endif

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void OnConstruction(const FTransform& Transform) override;
};

#if WITH_EDITORONLY_DATA
UCLASS()
class SPACEFORCE_API UAsteroidVisComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UAsteroidVisComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
#endif