// Fill out your copyright notice in the Description page of Project Settings.


#include "SFNavigationManager.h"
#include "Containers/Queue.h"
#include "DrawDebugHelpers.h"
#include "Landscape.h"
#include "SpaceForce.h"

ASFNavigationManager::ASFNavigationManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bDrawFailedNearestNavigableVoxels = false;
	MaxIterations = 200;

	VoxelSize = 150.0f;
	XGridSize = 514;
	YGridSize = 57;
	ZGridSize = 63;

	ObstacleQueryChannels.Add(ECC_WorldStatic);
	ObstacleQueryChannels.Add(ECC_WorldDynamic);
	ObstacleQueryChannels.Add(ECC_Destructible);

	AutoCorrectionGuessList.Reserve(4);
	AutoCorrectionGuessList.Add(200);
	AutoCorrectionGuessList.Add(375);
	AutoCorrectionGuessList.Add(500);
	AutoCorrectionGuessList.Add(1000);

	GenerateNeighborDirections();

	bEnableGroundScan = true;
	GroundTraceHeight = 200.0f;
}

void ASFNavigationManager::UpdateVoxelCollision(FDonNavigationVoxel& Volume)
{
	Super::UpdateVoxelCollision(Volume);
	if (bEnableGroundScan && Volume.CanNavigate())
	{
		Volume.SetNavigability(!IsVoxelBellowGround(Volume));
	}
}

bool ASFNavigationManager::IsVoxelBellowGround(FDonNavigationVoxel& Volume)
{
	//unsure if Voxel Location is centered
	FVector FloorOrigin = Volume.Location - 0.5 * VoxelSize * FVector(1, 1, 1);
	return IsPointBellowGround(FloorOrigin) ||
		IsPointBellowGround(FloorOrigin + VoxelSize * FVector(1, 0, 0)) ||
		IsPointBellowGround(FloorOrigin + VoxelSize * FVector(0, 1, 0)) ||
		IsPointBellowGround(FloorOrigin + VoxelSize * FVector(1, 1, 0));
}

bool ASFNavigationManager::IsPointBellowGround(const FVector& End) 
{
	TArray<FHitResult> Hits;
	FCollisionQueryParams QueryParams;
	FCollisionResponseParams ResponseParams;
	const FVector Start = End + FVector(0, 0, GroundTraceHeight);
	GetWorld()->LineTraceMultiByChannel(Hits, Start, End, COLLISION_LANDSCAPE, QueryParams, ResponseParams);
	
	if (Hits.Num()) 
	{
		if (bDebugGroundTrace) 
		{
			FColor Color = FColor::Red;
			DrawDebugPoint(GetWorld(), Start, 5, Color, false, 10.0f, 5);
			DrawDebugPoint(GetWorld(), End, 5, Color, false, 10.0f, 5);
			DrawDebugLine(GetWorld(), Start, End, Color, false, 10.0f, 5, 1.0f);
		}
		//UE_LOG(LogTemp, Warning, TEXT("IsPointBellowGround Result.Actor %s"), *Result.Actor.Get()->GetName())
		return true;
	}
	return false;
}

void ASFNavigationManager::GenerateNeighborDirections()
{
	TArray<FVector> Directions;
	Directions.Push(FVector(1, 0, 0));
	Directions.Push(FVector(2, 0, 0));
	Directions.Push(FVector(0, 1, 0));
	Directions.Push(FVector(1, 1, 0));
	Directions.Push(FVector(2, 1, 0));
	Directions.Push(FVector(0, 2, 0));
	Directions.Push(FVector(1, 2, 0));
	Directions.Push(FVector(2, 2, 0));
	Directions.Push(FVector(0, 0, 1));
	Directions.Push(FVector(1, 0, 1));
	Directions.Push(FVector(2, 0, 1));
	Directions.Push(FVector(0, 1, 1));
	Directions.Push(FVector(1, 1, 1));
	Directions.Push(FVector(2, 1, 1));
	Directions.Push(FVector(0, 2, 1));
	Directions.Push(FVector(1, 2, 1));
	Directions.Push(FVector(2, 2, 1));
	Directions.Push(FVector(0, 0, 2));
	Directions.Push(FVector(1, 0, 2));
	Directions.Push(FVector(2, 0, 2));
	Directions.Push(FVector(0, 1, 2));
	Directions.Push(FVector(1, 1, 2));
	Directions.Push(FVector(2, 1, 2));
	Directions.Push(FVector(0, 2, 2));
	Directions.Push(FVector(1, 2, 2));
	Directions.Push(FVector(2, 2, 2));
	NeighborDirections = Directions;
} 

bool ASFNavigationManager::IsLocationNavigable(FVector DesiredLocation)
{
	FDonNavigationVoxel* Voxel = VolumeAt(DesiredLocation);
	if (!Voxel->bIsInitialized)
		UpdateVoxelCollision(*Voxel);
	return Voxel->CanNavigate();
}

bool ASFNavigationManager::FindNearestNavigableVoxel(const FVector& Start, FVector& OutLocation)
{
	TQueue<FDonNavigationVoxel*> Queue;
	TSet<FDonNavigationVoxel*> Visited;

	FDonNavigationVoxel* CurrentVoxel = VolumeAt(Start);
	if (!CurrentVoxel) return false;

	Queue.Enqueue(CurrentVoxel);
	Visited.Add(CurrentVoxel);
	if (bDrawFailedNearestNavigableVoxels)
		DrawDebugPoint(GetWorld(), CurrentVoxel->Location, 5, FColor::Purple, false, 10.0f, 3);

	float Extent = 3.0f;
	int16 Iterations = MaxIterations;
	while (!Queue.IsEmpty() && Iterations > 0) {
		Iterations--;
		Queue.Dequeue(CurrentVoxel);

		if (!CurrentVoxel->bIsInitialized)
			UpdateVoxelCollision(*CurrentVoxel);

		bool bSuccess = CurrentVoxel->CanNavigate();

		if (bDrawFailedNearestNavigableVoxels)
			DrawDebugPoint(GetWorld(), CurrentVoxel->Location, 3, bSuccess ? FColor::Green : FColor::Red, false, 10.0f, 2);

		if (bSuccess)
		{
			OutLocation = CurrentVoxel->Location;
			UE_LOG(LogTemp, Warning, TEXT("FindNearestNavigableVoxel finished in %d with bSuccess SUCCESS"), (MaxIterations - Iterations))
			return true;
		}

		for (FVector Direction : NeighborDirections) 
		{
			FVector Neighbor = Extent * VoxelSize * (Direction - FVector(1, 1, 1)) + CurrentVoxel->Location;
			FDonNavigationVoxel* Next = VolumeAt(Neighbor);
			if (Next && !Visited.Contains(Next))
			{
				Queue.Enqueue(Next);
				Visited.Add(Next);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("FindNearestNavigableVoxel finished in %d with bSuccess FAIL"), (MaxIterations - Iterations))
	return false;
}