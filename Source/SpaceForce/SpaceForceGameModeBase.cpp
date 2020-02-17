// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SpaceForceGameModeBase.h"

ASpaceForceGameModeBase::ASpaceForceGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnOb(TEXT("/Game/Blueprints/PilotPawn"));
	DefaultPawnClass = PlayerPawnOb.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PilotPlayerControllerOb(TEXT("/Game/Blueprints/PilotPlayerController"));
	PlayerControllerClass = PilotPlayerControllerOb.Class;
}