// Fill out your copyright notice in the Description page of Project Settings.


#include "SFNavigationHelper.h"
#include "SFNavigationManager.h" 
#include "Helpers/LoggingHelper.h"

ASFNavigationManager* USFNavigationHelper::SFNavigationManager(UObject* WorldContextObject)
{
	auto* NavMan = Cast<ASFNavigationManager>(UDonNavigationHelper::DonNavigationManager(WorldContextObject));
	if (!NavMan)
	{
		UE_LOG(LogTemp, Error, TEXT("Called static SFNavigationManager but none exist in level."))
	}
	return NavMan;
}

ASFNavigationManager* USFNavigationHelper::SFNavigationManagerForActor(const AActor* Actor)
{
	auto* NavMan = Cast<ASFNavigationManager>(UDonNavigationHelper::DonNavigationManagerForActor(Actor));
	if (!NavMan)
	{
		UE_LOG(LogTemp, Error, TEXT("Called static SFNavigationManagerForActor(%s) but none exist in level."), *ULoggingHelper::GetNameOrNull(Actor))
	}
	return NavMan;
}