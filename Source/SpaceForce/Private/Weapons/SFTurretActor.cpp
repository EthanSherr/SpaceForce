#include "SFTurretActor.h"
#include "../Components/SFTurretComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "../Weapons/SFProjectile.h"
#include "Kismet/GameplayStatics.h"

ASFTurretActor::ASFTurretActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "Root");

	TurretComponent = ObjectInitializer.CreateDefaultSubobject<USFTurretComponent>(this, "Turret");
	TurretComponent->SetupAttachment(RootComponent);

	ProjectileSpeedOverride = 0.0f;
}

void ASFTurretActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (ProjectileSpeedOverride)
	{
		TurretComponent->ProjectileSpeed = ProjectileSpeedOverride;
	}
}

void ASFTurretActor::AimAt(FVector Target)
{
	TurretComponent->AimAt(Target);
}

void ASFTurretActor::AimAtActor(AActor* Actor)
{
	TurretComponent->AimAtActor(Actor);
}

void ASFTurretActor::TriggerAction_Implementation(bool bIsPressed)
{
	if (bIsPressed)
	{
		FTransform MuzzleTransform = TurretComponent->GetMuzzleTransform();
		MuzzleTransform.RemoveScaling();
		SpawnProjectile(MuzzleTransform);
	}
}

float ASFTurretActor::GetTriggetAxis()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
		return 0.0f;
	return PC->GetInputAxisKeyValue("TriggerAxis");
}

void ASFTurretActor::SpawnProjectile(const FTransform& Transform)
{
	if (!ProjectileClass)
		return;
	ASFProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASFProjectile>(ProjectileClass, Transform);
	// setup projectile configuration pre-spawn
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	if (GetOwner())
	{
		IgnoreActors.Add(GetOwner());
	}
	Projectile->IgnoreActors = IgnoreActors;
	if (ProjectileSpeedOverride)
	{
		Projectile->Speed = ProjectileSpeedOverride;
	}

	UAudioComponent* AC = NULL;
	if (FireSound)
	{
		AC = UGameplayStatics::SpawnSoundAttached(FireSound, TurretComponent);
	}

	UGameplayStatics::FinishSpawningActor(Projectile, Transform);
}