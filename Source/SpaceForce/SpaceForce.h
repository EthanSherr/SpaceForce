// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define COLLISION_PROJECTILE	ECC_GameTraceChannel1
#define COLLISION_GENERATOR 	ECC_GameTraceChannel2
#define COLLISION_LANDSCAPE		ECC_GameTraceChannel3


#define COLLISION_PROFILE_BLOCK_ALL_DYNAMIC FName("BlockAllDynamic")
#define COLLISION_PROFILE_TRIGGER			FName("Trigger")
#define COLLISION_PROFILE_PAWN				FName("Pawn")
#define COLLISION_PROFILE_DESTRUCTIBLE		FName("Destructible")
#define COLLISION_PROFILE_GENERATOR			FName("Generator")
#define COLLISION_PROFILE_LANDSCAPE			FName("Landscape")
#define COLLISION_PROFILE_UI				FName("UI")