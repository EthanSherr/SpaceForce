// Fill out your copyright notice in the Description page of Project Settings.


#include "SFBrushMeshGenBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "SpaceForce.h"
#include "SFMeshBrushGenComponent.h"
#include "UObject/ConstructorHelpers.h"

ASFBrushMeshGenBase::ASFBrushMeshGenBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	Root = RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	BrushGenerator = ObjectInitializer.CreateDefaultSubobject<USFMeshBrushGenComponent>(this, TEXT("BrushMesh"));
	BrushGenerator->SetupAttachment(RootComponent);
	Count = 15;
	bGenerateOnConstruction = true;
}

void ASFBrushMeshGenBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (bGenerateOnConstruction)
	{
		GenerateTemplates();
	}
}

void ASFBrushMeshGenBase::GenerateTemplates() {
	if (!Templates.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("No template StaticMesh provided."))
		return;
	}
	for (int i = 0; i < Count; i++) 
	{
		FTransform Transform;
		bool bSuccessfulLocation = BrushGenerator && BrushGenerator->GenerateTransform_Implementation(Transform);
		if (!bSuccessfulLocation)
			continue;

		FVector Velocity, AngularVelocity;
		GeneratePhysics_Implementation(Velocity, AngularVelocity);
		InstantiateMesh(Transform, Velocity, AngularVelocity);
	}
}

void ASFBrushMeshGenBase::GeneratePhysics_Implementation(FVector& OutVelocity, FVector& OutAngularVelocity)
{

}

void ASFBrushMeshGenBase::InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity)
{

}

