// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SFCampaignSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API USFCampaignSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FString, bool> UnlockedLevels;
};
