// Fill out your copyright notice in the Description page of Project Settings.


#include "SFMeshGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "SpaceForce.h"

// Sets default values
ASFMeshGenerator::ASFMeshGenerator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	ShapeMeshExterior = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ShapeMeshExteroir"));
	ShapeMeshInterior = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ShapeMeshInterior"));
	ShapeMeshExterior->SetupAttachment(RootComponent);
	ShapeMeshInterior->SetupAttachment(RootComponent);

	ShapeMeshInterior->SetCollisionProfileName(FName(COLLISION_PROFILE_GENERATOR));
	ShapeMeshExterior->SetCollisionProfileName(FName(COLLISION_PROFILE_GENERATOR));
	Count = 15;

	MinScale = 1.0f;
	MaxScale = 1.5f;
}


void ASFMeshGenerator::OnConstruction(const FTransform& Transform) {
	ShapeMeshInterior->SetStaticMesh(InteriorMesh);
	ShapeMeshExterior->SetStaticMesh(ExteriorMesh);

	DrawDebugBox(GetWorld(), GetActorLocation(), ShapeMeshExterior->Bounds.BoxExtent, FColor::Black, false, 30, 5, 1);
	GenerateTemplates();
}

void ASFMeshGenerator::Tick(float DeltaTime) {
	//GenerateTemplates();
}


void ASFMeshGenerator::GenerateTemplates() {
	float debugTime = 30;
	FVector Extent = ShapeMeshExterior->Bounds.BoxExtent;
	FVector Origin = ShapeMeshExterior->Bounds.Origin;
	FVector Exterior = Origin + 2 * Extent;
	for (int i = 0; i < Count; i++) {
		FVector BoxInterior = FVector(
			Stream.FRandRange(-Extent.X, Extent.X),
			Stream.FRandRange(-Extent.Y, Extent.Y),
			Stream.FRandRange(-Extent.Z, Extent.Z)) + Origin;
		
		TArray<FHitResult> HitResults;
		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		FCollisionResponseParams ResponseParams;
		int Intersections = 0;
		GetWorld()->LineTraceMultiByChannel(HitResults, BoxInterior, Exterior, COLLISION_GENERATOR, Params);
		for (FHitResult Result : HitResults) {
			Intersections += (Result.GetComponent() == ShapeMeshExterior || Result.GetComponent() == ShapeMeshInterior) ? 1 : 0;
		}
		
		bool bIsInside = Intersections % 2 == 1;
		if (bIsInside) {
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
			Mesh->RegisterComponentWithWorld(GetWorld());
			Mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			Mesh->SetWorldLocation(BoxInterior, false, NULL, ETeleportType::TeleportPhysics);
			Mesh->SetStaticMesh(TemplateMesh);

			FTransform Transform = FTransform(
				FRotator(Stream.FRand() * 360, Stream.FRand() * 360, Stream.FRand() * 360), 
				BoxInterior, 
				FVector(Stream.FRandRange(MinScale, MaxScale)));
			Mesh->SetWorldTransform(Transform, false, NULL, ETeleportType::TeleportPhysics);
		}
	}
}


