// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SFTransformGenerator.h"
#include "SFMeshBrushGenerator.generated.h"

class UStaticMeshComponent;
class UStaticMesh;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFMeshBrushGenerator : public UStaticMeshComponent, public ISFTransformGenerator
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Generation")
	FRandomStream Stream;

	UPROPERTY(EditAnywhere, Category = "Generation|Ranges")
	float MinScale;

	UPROPERTY(EditAnywhere, Category = "Generation|Ranges")
	float MaxScale;

	UPROPERTY(EditAnywhere, Category = "Generation|Advanced")
	float TunnelingDelta;

	UPROPERTY(EditAnywhere, Category = "Generation|Advanced")
	int MaxIterations;
		
	UPROPERTY(EditAnywhere, Category = "Generation|Debug")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Generation|Debug")
	bool bDebugLines;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GenerateTransform(FTransform& OutTransform);

private:
	int Intersections(FVector Beginning, FVector End);

};
