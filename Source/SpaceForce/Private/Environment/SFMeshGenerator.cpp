// Fill out your copyright notice in the Description page of Project Settings.


#include "SFMeshGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "SpaceForce.h"

// Sets default values
ASFMeshGenerator::ASFMeshGenerator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	ShapeMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ShapeMesh"));
	ShapeMesh->SetupAttachment(RootComponent);

	ShapeMesh->SetCollisionProfileName(FName(COLLISION_PROFILE_GENERATOR));
	ShapeMesh->SetHiddenInGame(true);
	Count = 15;

	MinScale = 1.0f;
	MaxScale = 1.5f;

	TunnelingDelta = 0.2f;
	MaxIterations = 10;
}


void ASFMeshGenerator::OnConstruction(const FTransform& Transform) {
	ShapeMesh->SetStaticMesh(BrushMesh);
	if (bDebug) {
		DrawDebugBox(GetWorld(), GetActorLocation(), ShapeMesh->Bounds.BoxExtent, FColor::Black, false, 30, 5, 1);
	}
	GenerateTemplates();
}


void ASFMeshGenerator::GenerateTemplates() {
	float debugTime = 30;
	FVector Extent = ShapeMesh->Bounds.BoxExtent;
	FVector Origin = ShapeMesh->Bounds.Origin;
	FVector Exterior = Origin + Extent * 1.5;
	for (int i = 0; i < Count; i++) {
		FVector BoxInterior = FVector(
			Stream.FRandRange(-Extent.X, Extent.X),
			Stream.FRandRange(-Extent.Y, Extent.Y),
			Stream.FRandRange(-Extent.Z, Extent.Z)) + Origin;
		bool bIsInside = Intersections(BoxInterior, Exterior) % 2 == 1;
		if (bDebug) {
			DrawDebugPoint(GetWorld(), BoxInterior, 15, bIsInside ? FColor::Blue : FColor::Red, false, 50, 5);
		}

		if (bIsInside && !bDebug) {
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

int ASFMeshGenerator::Intersections(FVector Interior, FVector End) {
	FVector Beginning = Interior;
	int TotalHits = 0;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	int ScanCount = 0;
	while (ScanCount < MaxIterations) {
		TArray<FHitResult> HitResults;
		GetWorld()->LineTraceMultiByChannel(HitResults, Beginning, End, COLLISION_GENERATOR, Params);
		int Hits = 0;
		for (FHitResult Result : HitResults) {
			bool bIsHit = Result.GetComponent() == ShapeMesh;

			if (bIsHit) {
				if (bDebug) {
					DrawDebugPoint(GetWorld(), Result.Location, 15, FColor::Purple, false, 50, 5);
				}
				if (bDebugLines) {
					DrawDebugLine(GetWorld(), Beginning, Result.Location, FColor::Black, false, 50, 5, 1);
				}
				Beginning = Result.Location + TunnelingDelta * (End - Beginning).GetSafeNormal();
				Hits++;
			}
		}
		if (Hits == 0) {
			if (bDebugLines) {
				DrawDebugLine(GetWorld(), Beginning, End, FColor::Yellow, false, 50, 5, 1);
			}
			break;
		}
		TotalHits += Hits;
		ScanCount++;
	}
	return TotalHits;
}