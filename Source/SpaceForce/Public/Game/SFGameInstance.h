// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SFGameInstance.generated.h"

class USFCampaignSaveGame;

UCLASS()
class SPACEFORCE_API USFGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void UnlockLevel(FString LevelName);

    UFUNCTION(BlueprintCallable)
    void GetUnlockedLevels(TMap<FString, bool>& OutLevels);

    UPROPERTY(BlueprintReadWrite)
    FString Slot;

private:
    USFCampaignSaveGame* GetCampaignForSlot(FString InSlot);
};
