// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SplineMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFFlightPath.generated.h"

class USplineComponent;

UCLASS()
class SPACEFORCE_API ASFFlightPath : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
		USplineComponent* Spline;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ESplineMeshAxis::Type> splineForward;

	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
		float bucketSize;

	UPROPERTY(EditAnywhere)
		int columns;

	UPROPERTY(EditAnywhere)
		int rows;


	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetupSpline();

};
