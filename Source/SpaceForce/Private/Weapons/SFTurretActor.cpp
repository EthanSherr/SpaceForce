#include "SFTurretActor.h"
#include "../Components/SFTurretComponent.h"

// Sets default values
ASFTurretActor::ASFTurretActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	TurretComponent = ObjectInitializer.CreateDefaultSubobject<USFTurretComponent>(this, "Turret");
	TurretComponent->SetupAttachment(GetRootComponent());
}

void ASFTurretActor::AimAt(FVector Target)
{
	TurretComponent->AimAt(Target);
}

void ASFTurretActor::AimAtActor(AActor* Actor)
{
	TurretComponent->AimAtActor(Actor);
}

