// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceForceGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API ASpaceForceGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()
	
	//give the player the pawn
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
