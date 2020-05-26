// Fill out your copyright notice in the Description page of Project Settings.


#include "SFMeshGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "SpaceForce.h"
#include "SFMeshBrushGenerator.h"

// Sets default values
ASFMeshGenerator::ASFMeshGenerator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	BrushGenerator = ObjectInitializer.CreateDefaultSubobject<USFMeshBrushGenerator>(this, TEXT("BrushMesh"));
	BrushGenerator->SetupAttachment(RootComponent);
	Count = 15;
}

void ASFMeshGenerator::OnConstruction(const FTransform& Transform) 
{
	Super::OnConstruction(Transform);
	GenerateTemplates();
}

void ASFMeshGenerator::GenerateTemplates() {
	if (!Templates.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("No template StaticMesh provided."))
		return;
	}
	for (int i = 0; i < Count; i++) 
	{
		FTransform Transform;
		bool bSuccessfulLocation = BrushGenerator->GenerateTransform_Implementation(Transform);
		if (!bSuccessfulLocation)
			continue;

		FVector Velocity, AngularVelocity;
		InstantiateMesh(Transform, Velocity, AngularVelocity);
	}
}

void ASFMeshGenerator::GeneratePhysics_Implementation(FVector& OutVelocity, FVector& OutAngularVelocity)
{

}

void ASFMeshGenerator::InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity)
{

}
