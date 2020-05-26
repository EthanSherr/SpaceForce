// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFTransformGenerator.h"
#include "SFPhysicsGenerator.h"
#include "SFMeshGenerator.generated.h"

class USFMeshBrushGenerator;
class UStaticMesh;

UCLASS()
class SPACEFORCE_API ASFMeshGenerator : public AActor, public ISFPhysicsGenerator
{
	GENERATED_UCLASS_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Generation")
	FRandomStream Stream;

	UPROPERTY(EditAnywhere)
	USFMeshBrushGenerator* BrushGenerator;

	UPROPERTY(EditAnywhere, Category = "Generation")
	int Count;

	UPROPERTY(EditAnywhere, Category = "Generation")
	TArray<UStaticMesh*> Templates;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GeneratePhysics(FVector& OutVelocity, FVector& OutAngularVelocity);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateTemplates();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual void InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity);
};
