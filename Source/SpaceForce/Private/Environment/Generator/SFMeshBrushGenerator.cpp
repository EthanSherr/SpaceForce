#include "DrawDebugHelpers.h"
#include "SpaceForce.h"
#include "SFMeshBrushGenerator.h"

// Sets default values for this component's properties
USFMeshBrushGenerator::USFMeshBrushGenerator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCollisionProfileName(FName(COLLISION_PROFILE_GENERATOR));
	SetHiddenInGame(true);

	MinScale = 1.0f;
	MaxScale = 1.5f;

	TunnelingDelta = 0.2f;
	MaxIterations = 10;
}

bool USFMeshBrushGenerator::GenerateTransform_Implementation(FTransform& OutTransform)
{
	FVector Extent = Bounds.BoxExtent;
	FVector Origin = Bounds.Origin;
	FVector Exterior = Origin + Extent * 1.5;
	FVector BoxInterior = FVector(
		Stream.FRandRange(-Extent.X, Extent.X),
		Stream.FRandRange(-Extent.Y, Extent.Y),
		Stream.FRandRange(-Extent.Z, Extent.Z)) + Origin;

	bool bIsInside = Intersections(BoxInterior, Exterior) % 2 == 1;
	if (bDebug)
		DrawDebugPoint(GetWorld(), BoxInterior, 15, bIsInside ? FColor::Blue : FColor::Red, false, 50, 5);

	if (!bIsInside)
		return false;

	OutTransform = FTransform(
		FRotator(Stream.FRand() * 360, Stream.FRand() * 360, Stream.FRand() * 360),
		BoxInterior,
		FVector(Stream.FRandRange(MinScale, MaxScale)));

	return true;
}

int USFMeshBrushGenerator::Intersections(FVector Interior, FVector End) {
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
			bool bIsHit = Result.GetComponent() == this;

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