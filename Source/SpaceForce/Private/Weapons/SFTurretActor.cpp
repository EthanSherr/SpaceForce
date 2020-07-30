#include "SFTurretActor.h"
#include "../Components/SFTurretComponent.h"
#include "Components/SkeletalMeshComponent.h"

ASFTurretActor::ASFTurretActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "Root");

	TurretComponent = ObjectInitializer.CreateDefaultSubobject<USFTurretComponent>(this, "Turret");
	TurretComponent->SetupAttachment(RootComponent);
}

void ASFTurretActor::AimAt(FVector Target)
{
	TurretComponent->AimAt(Target);
}

void ASFTurretActor::AimAtActor(AActor* Actor)
{
	TurretComponent->AimAtActor(Actor);
}

