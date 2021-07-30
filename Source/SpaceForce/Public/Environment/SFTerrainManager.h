// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFTerrainManager.generated.h"

USTRUCT(Blueprintable)
struct SPACEFORCE_API FTerrainSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Number of Components"))
	FIntPoint ComponentCount;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Sections Per Component"))
	int32 SectionsPerComponent;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Section Size"))
	int32 QuadsPerSection;

	UPROPERTY(BlueprintReadWrite)
	bool bCanHaveLayersContent;

	UPROPERTY(BlueprintReadWrite)
	FVector Scale;

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	FString HeightmapFilename;
};

UCLASS()
class SPACEFORCE_API ASFTerrainManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFTerrainManager(const FObjectInitializer& ObjectInitializer);

	//UFUNCTION(BlueprintCallable)
	//void GenerateTerrain(FTerrainSettings Settings);
};
