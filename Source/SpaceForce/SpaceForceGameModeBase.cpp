// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SpaceForceGameModeBase.h"
#include "GameFramework/PlayerController.h"

ASpaceForceGameModeBase::ASpaceForceGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnOb(TEXT("/Game/Blueprints/PilotPawn"));
	//DefaultPawnClass = PlayerPawnOb.Class;

	//static ConstructorHelpers::FClassFinder<APlayerController> PilotPlayerControllerOb(TEXT("/Game/Blueprints/PilotPlayerController"));
	//PlayerControllerClass = PilotPlayerControllerOb.Class;

	UE_LOG(LogTemp, Warning, TEXT("Quaternion tests:"))

	FVector v1 = FVector::ForwardVector;
	FVector v2 = FVector::RightVector;

	auto quat = FQuat::FindBetween(v1, v2);

	UE_LOG(LogTemp, Warning, TEXT("quat => %s"), *quat.ToString())
	UE_LOG(LogTemp, Warning, TEXT("Tests done"))
}

void ASpaceForceGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}