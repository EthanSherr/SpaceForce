// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFTransformGenerator.h"
#include "SFPhysicsGenerator.h"
#include "SFBrushMeshGenBase.generated.h"

class USFMeshBrushGenComponent;
class USceneComponent;
class UStaticMesh;

UCLASS()
class SPACEFORCE_API ASFBrushMeshGenBase : public AActor, public ISFPhysicsGenerator
{
	GENERATED_UCLASS_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category = "Generation")
	FRandomStream Stream;

	UPROPERTY(EditAnywhere)
	USFMeshBrushGenComponent* BrushGenerator;

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

	virtual void InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity);

	UPROPERTY(EditAnywhere)
	bool bGenerateOnConstruction;
};

