// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../SFTriggerableActor.h"
#include "CoreMinimal.h"
#include "Environment/Generator/SFBrushMeshGenBase.h"
#include "SFAsteroidBrush.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API ASFAsteroidBrush : public ASFBrushMeshGenBase, public ISFTriggerableActor
{
	GENERATED_UCLASS_BODY()

public:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

	virtual void InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity) override;

	UPROPERTY(EditAnywhere, Category = "Generator")
	float MinVelocityMagnitude;

	UPROPERTY(EditAnywhere, Category = "Generator")
	float MaxVelocityMagnitude;
	
	UPROPERTY(EditAnywhere, Category = "Generator")
	float VariationDegrees;

	UPROPERTY(EditAnywhere, Category = "Generator")
	FVector VelocityDirection;

	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> GeneratedActors;

	UPROPERTY(EditInstanceOnly)
	bool bCheckToGenerate;

	UPROPERTY(EditInstanceOnly)
	bool bDetachGenerated;

	virtual void GeneratePhysics_Implementation(FVector& OutVelocity, FVector& OutAngularVelocity) override;

};
