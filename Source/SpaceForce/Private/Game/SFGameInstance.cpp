// Fill out your copyright notice in the Description page of Project Settings.

#include "SFGameInstance.h"
#include "../Game/SFCampaignSaveGame.h"
#include "Kismet/GameplayStatics.h"

USFGameInstance::USFGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Slot = FString("MySaveSlot");
}

void USFGameInstance::UnlockLevel(const FName& LevelName)
{
	if (LevelName == FName()) 
	{
		UE_LOG(LogTemp, Error, TEXT("LevelName is None, cannot unlock level."))
		return;
	}
	auto* CampaignSaveGame = GetCampaignForSlot(Slot);
	CampaignSaveGame->UnlockedLevels.Add(LevelName, true);
	UGameplayStatics::SaveGameToSlot(CampaignSaveGame, *Slot, 0);
}

void USFGameInstance::GetUnlockedLevels(TMap<FName, bool>& OutLevels)
{
	auto* CampaignSaveGame = GetCampaignForSlot(Slot);
	OutLevels = CampaignSaveGame->UnlockedLevels;
}

USFCampaignSaveGame* USFGameInstance::GetCampaignForSlot(const FString& InSlot)
{
	auto* Campaign = Cast<USFCampaignSaveGame>(UGameplayStatics::LoadGameFromSlot(Slot, 0));
	if (!Campaign)
	{
		Campaign = Cast<USFCampaignSaveGame>(UGameplayStatics::CreateSaveGameObject(USFCampaignSaveGame::StaticClass()));
	}
	return Campaign;
}
