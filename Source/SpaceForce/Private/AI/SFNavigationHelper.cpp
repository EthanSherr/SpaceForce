// Fill out your copyright notice in the Description page of Project Settings.


#include "SFNavigationHelper.h"
#include "SFNavigationManager.h" 

ASFNavigationManager* USFNavigationHelper::SFNavigationManager(UObject* WorldContextObject)
{
	return Cast<ASFNavigationManager>(UDonNavigationHelper::DonNavigationManager(WorldContextObject));
}

ASFNavigationManager* USFNavigationHelper::SFNavigationManagerForActor(const AActor* Actor)
{
	return Cast<ASFNavigationManager>(UDonNavigationHelper::DonNavigationManagerForActor(Actor));
}