// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFMeshGenerator.generated.h"

UCLASS()
class SPACEFORCE_API ASFMeshGenerator : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Generation")
	FRandomStream Stream;

	class UStaticMeshComponent* ShapeMesh;

	UPROPERTY(EditAnywhere, Category = "Generation")
	int Count;

	UPROPERTY(EditAnywhere, Category = "Generation")
	class UStaticMesh* TemplateMesh;

	UPROPERTY(EditAnywhere, Category = "Generation")
	class UStaticMesh* BrushMesh;

	UPROPERTY(EditAnywhere, Category = "Generation|Advanced")
	float TunnelingDelta;

	UPROPERTY(EditAnywhere, Category = "Generation|Advanced")
	int MaxIterations;

	UPROPERTY(EditAnywhere, Category = "Generation|Ranges")
	float MinScale;

	UPROPERTY(EditAnywhere, Category = "Generation|Ranges")
	float MaxScale;

	UPROPERTY(EditAnywhere, Category = "Generation|Debug")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Generation|Debug")
	bool bDebugLines;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	int Intersections(FVector Beginning, FVector End);

private:
	void GenerateTemplates();

};
