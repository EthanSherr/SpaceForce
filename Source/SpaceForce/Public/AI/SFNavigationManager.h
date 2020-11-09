#pragma once

#include "CoreMinimal.h"
#include "DonNavigationManager.h"
#include "SFNavigationManager.generated.h"

UCLASS()
class SPACEFORCE_API ASFNavigationManager : public ADonNavigationManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SFNav|Debug")
	bool bDrawFailedNearestNavigableVoxels;

	UPROPERTY(EditAnywhere, Category = "SFNav|Debug")
	bool bDebugGroundTrace;

	UPROPERTY(EditAnywhere, Category = "SFNav|BFS Settings")
	int16 MaxIterations;

	UPROPERTY(EditAnywhere, Category = "SFNav|Ground")
	bool bEnableGroundScan;

	UPROPERTY(EditAnywhere, Category = "SFNav|Ground")
	float GroundTraceHeight;
	
public:
	ASFNavigationManager(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "SFNavigation Custom")
	bool IsLocationNavigable(FVector DesiredLocation);

	UFUNCTION(BlueprintPure, Category = "SFNavigation Custom")
	bool FindNearestNavigableVoxel(const FVector& Start, FVector& OutLocation);

private:
	TArray<FVector> NeighborDirections;
	void GenerateNeighborDirections();

	// Helper function for UpdateVoxelCollision's GroundScan option if bEnableGroundScan
	// Helps AI avoid picking points that are bellow the Landscape
	bool IsVoxelBellowGround(FDonNavigationVoxel& Volume);
	bool IsPointBellowGround(const FVector& End);
protected:
	void UpdateVoxelCollision(FDonNavigationVoxel& Volume) override;

};
