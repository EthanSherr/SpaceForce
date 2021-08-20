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
	bool IsLevlUnlocked = UnlockedLevels.Contains(LevelName);
	UE_LOG(LogTemp, Warning, TEXT("ASFCampaignGameMode Level %s is unlocked: %d"), *LevelName.ToString(), IsLevlUnlocked)
	return UnlockedLevels.Contains(LevelName);
}
