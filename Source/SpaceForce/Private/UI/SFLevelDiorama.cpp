#include "SFLevelDiorama.h"
#include "Components/SphereComponent.h"
#include "SpaceForce.h"
#include "../Game/SFCampaignGameMode.h"
#include "SFLevelDiorama.h"
// Sets default values
ASFLevelDiorama::ASFLevelDiorama(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionSphere"));
	CollisionSphere->SetCollisionProfileName(COLLISION_PROFILE_UI);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASFLevelDiorama::BeginPlay()
{
	Super::BeginPlay();
	auto* CampaignGM = GetWorld()->GetAuthGameMode<ASFCampaignGameMode>();
	SetEnabled(bForceEnabled || CampaignGM->IsLevelUnlocked(Level));
}

FName ASFLevelDiorama::GetLevel()
{
	return Level;
}

bool ASFLevelDiorama::GetFocused()
{
	return bFocused;
}

bool ASFLevelDiorama::GetSelected()
{
	return bSelected;
}

bool ASFLevelDiorama::GetEnabled()
{
	return bEnabled;
}

void ASFLevelDiorama::SetSelected(bool InSelected)
{
	if (bSelected == InSelected)
		return;
	bSelected = InSelected;
	ReceiveOnSelectedChanged(InSelected);
	if (bSelected)
	{
		OnLevelSelected.Broadcast(this);
	}
}

void ASFLevelDiorama::SetFocused(bool InFocused)
{
	if (bFocused == InFocused)
		return;
	
	bFocused = InFocused;
	ReceiveOnFocusChanged(InFocused);
}

void ASFLevelDiorama::SetEnabled(bool InEnabled)
{
	if (bEnabled == InEnabled)
		return;
	bEnabled = InEnabled;
	ReceiveOnEnabledChanged(InEnabled);
}