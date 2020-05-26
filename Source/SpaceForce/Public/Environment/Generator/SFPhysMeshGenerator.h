// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SFBrushMeshGenBase.h"
#include "SFPhysMeshGenerator.generated.h"

UCLASS()
class SPACEFORCE_API ASFPhysMeshGenerator : public ASFBrushMeshGenBase
{
	GENERATED_BODY()

public:

	virtual void InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity) override;

};
