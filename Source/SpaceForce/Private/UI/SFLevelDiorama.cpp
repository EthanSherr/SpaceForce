// Fill out your copyright notice in the Description page of Project Settings.


#include "SFLevelDiorama.h"
#include "Components/SphereComponent.h"
#include "SpaceForce.h"
// Sets default values
ASFLevelDiorama::ASFLevelDiorama(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionSphere"));
	CollisionSphere->SetCollisionProfileName(COLLISION_PROFILE_UI);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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

void ASFLevelDiorama::SetSelected(bool InSelected)
{
	bSelected = InSelected;
	ReceiveOnSelectedChanged(InSelected);
	if (bSelected)
	{
		OnLevelSelected.Broadcast(this);
	}
}

void ASFLevelDiorama::SetFocused(bool InFocused)
{
	bFocused = InFocused;
	ReceiveOnFocusChanged(InFocused);
}