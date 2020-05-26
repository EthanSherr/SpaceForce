#include "SFCampaignGameMode.h"
#include "SFGameInstance.h"

void ASFCampaignGameMode::BeginPlay()
{
	Super::BeginPlay();	
	LoadUnlockedLevels();
}

void ASFCampaignGameMode::LoadUnlockedLevels()
{
	auto* SFGI = GetGameInstance<USFGameInstance>();
	SFGI->GetUnlockedLevels(UnlockedLevels);
}

bool ASFCampaignGameMode::IsLevelUnlocked(const FName& LevelName)
{
	return UnlockedLevels.Contains(LevelName);
}
