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

	class UStaticMeshComponent* ShapeMeshExterior;

	class UStaticMeshComponent* ShapeMeshInterior;

	UPROPERTY(EditAnywhere, Category = "Generation")
	int Count;

	UPROPERTY(EditAnywhere, Category = "Generation")
	class UStaticMesh* TemplateMesh;

	UPROPERTY(EditAnywhere, Category = "Generation")
	class UStaticMesh* ExteriorMesh;

	UPROPERTY(EditAnywhere, Category = "Generation")
	class UStaticMesh* InteriorMesh;

	UPROPERTY(EditAnywhere, Category = "Generation|Ranges")
	float MinScale;

	UPROPERTY(EditAnywhere, Category = "Generation|Ranges")
	float MaxScale;



protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaTime) override;

private:
	void GenerateTemplates();

};
