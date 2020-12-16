// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SFPlayerShip.h"
#include "Components/SFBoosterManagerComponent.h"


ASFPlayerShip::ASFPlayerShip(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	BoosterManagerComponent = ObjectInitializer.CreateDefaultSubobject<USFBoosterManagerComponent>(this, FName("BoosterManagerComponent"));
}

void ASFPlayerShip::TrySetIsBoosting(bool bNewIsBoosting)
{
	BoosterManagerComponent->TrySetIsBoosting(bNewIsBoosting);
}