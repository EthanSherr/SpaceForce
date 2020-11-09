// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DonNavigationHelper.h"
#include "SFNavigationHelper.generated.h"

class ASFNavigationManager;

UCLASS()
class SPACEFORCE_API USFNavigationHelper : public UDonNavigationHelper
{
	GENERATED_BODY()
	
public:

	/* Returns the Voxel navigation builder used for building navigation volumes and performing pathfinding */
	UFUNCTION(BlueprintPure, Category = "SFNavigation", meta = (WorldContext = "WorldContextObject"))
	static ASFNavigationManager* SFNavigationManager(UObject* WorldContextObject);

	/* Returns the Voxel navigation builder used for building navigation volumes and performing pathfinding for specified Actor*/
	UFUNCTION(BlueprintPure, Category = "SFNavigation")
	static ASFNavigationManager* SFNavigationManagerForActor(const AActor* Actor);

};
