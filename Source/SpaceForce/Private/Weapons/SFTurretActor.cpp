#include "SFTurretActor.h"
#include "../Components/SFTurretComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "../Weapons/SFProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "SFAimVisualization.h"
#include "Helpers/LoggingHelper.h"

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

	UE_LOG(LogTemp, Warning, TEXT("PostInitializeComonents %s AimVisTemp: %s"), *GetName(), *ULoggingHelper::GetNameOrNull(AimVisualizationTemplate))

	if (AimVisualizationTemplate)
	{
		//TODO load muzzle transform lazily, instead of initialization step.
		TurretComponent->Initialize();
		FTransform Transform = TurretComponent->GetMuzzleTransform();
		AimVisualization = GetWorld()->SpawnActorDeferred<ASFAimVisualization>(AimVisualizationTemplate, Transform);
		AimVisualization->Turret = this;
		AimVisualization->FinishSpawning(Transform);
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		AimVisualization->AttachToComponent(TurretComponent->SkeletalMesh, AttachmentRules, TurretComponent->MuzzleName);
	}
}

//void ASFTurretActor::PostEditChangeProperty(struct FPropertyChangedEvent& e)
//{
//	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
//	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASFTurretActor, AimVisualizationTemplate))
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AimVisualizationTemplate changed"))
//		FTransform Transform;
//		AimVisualization = GetWorld()->SpawnActorDeferred<ASFAimVisualization>(AimVisualizationTemplate, Transform);
//		AimVisualization->FinishSpawning(Transform);
//	}
//	Super::PostEditChangeProperty(e);
//}

void ASFTurretActor::AimAt(FVector Target)
{
	TurretComponent->AimAt(Target);
	if (AimVisualization) AimVisualization->SetTargetLocation(Target);
}

void ASFTurretActor::AimAtComponent(USceneComponent* SceneComponent)
{
	TurretComponent->AimAtComponent(SceneComponent);
	if (AimVisualization) AimVisualization->SetTargetComponent(SceneComponent);
}

void ASFTurretActor::TriggerAction_Implementation(bool bIsPressed)
{
	if (bIsPressed)
	{
		SpawnProjectile(TurretComponent->GetMuzzleTransform());
	}
}

float ASFTurretActor::GetTriggetAxis()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
		return 0.0f;
	return PC->GetInputAxisKeyValue("TriggerAxis");
}

ASFProjectile* ASFTurretActor::SpawnProjectile(const FTransform& Transform)
{
	if (!ProjectileClass)
		return NULL;
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

	return Projectile;
}

void ASFTurretActor::SetActivated(bool bValue)
{
	ReceiveActivated(bValue);
	if (AimVisualization)
	{
		AimVisualization->SetActivated(bValue);
	}

	if (!bValue) 
	{
		AimAtComponent(NULL);
	}

	if (ActivationSound && bValue)
	{
		UGameplayStatics::SpawnSoundAttached(ActivationSound, TurretComponent);
	}
}