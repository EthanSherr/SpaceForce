// Fill out your copyright notice in the Description page of Project Settings.

#include "../Game/SFCampaignSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SFGameInstance.h"

USFGameInstance::USFGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Slot = FString("MySaveSlot");
}

void USFGameInstance::UnlockLevel(FString LevelName)
{
	auto* CampaignSaveGame = GetCampaignForSlot(Slot);
	CampaignSaveGame->UnlockedLevels.Add(LevelName, true);
	UGameplayStatics::SaveGameToSlot(CampaignSaveGame, *Slot, 0);
}

void USFGameInstance::GetUnlockedLevels(TMap<FString, bool>& OutLevels)
{
	auto* CampaignSaveGame = GetCampaignForSlot(Slot);
	OutLevels = CampaignSaveGame->UnlockedLevels;
}

USFCampaignSaveGame* USFGameInstance::GetCampaignForSlot(FString InSlot)
{
	auto* Campaign = Cast<USFCampaignSaveGame>(UGameplayStatics::LoadGameFromSlot(Slot, 0));
	if (!Campaign)
	{
		Campaign = Cast<USFCampaignSaveGame>(UGameplayStatics::CreateSaveGameObject(USFCampaignSaveGame::StaticClass()));
	}
	return Campaign;
}
