// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SFCampaignGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API ASFCampaignGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	void LoadUnlockedLevels();

private:
	TMap<FName, bool> UnlockedLevels;

public:
	UFUNCTION(BlueprintCallable)
	bool IsLevelUnlocked(const FName& Level);
};
