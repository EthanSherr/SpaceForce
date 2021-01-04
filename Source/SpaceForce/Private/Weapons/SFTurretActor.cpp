#include "SFTurretActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SFAimVisualization.h"
#include "SFProjectile.h"
#include "SFSimpleTurretAnimInstance.h"

ASFTurretActor::ASFTurretActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "Root");

	TurretComp = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "TurretComp");
	TurretComp->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstOb(TEXT("/Game/Assets/skm/Ship_Turret_Base/ABP_Ship_Turret_Base1"));
	TurretComp->AnimClass = AnimInstOb.Class;

	MuzzleName = FName("Muzzle");
	BarrelName = FName("Barrel");

	ProjectileSpeedOverride = 0.0f;
}

void ASFTurretActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ASFTurretActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileSpeed = ProjectileSpeedOverride;
	if (ProjectileSpeed <= 0 && ProjectileClass)
	{
		ProjectileSpeed = ProjectileClass.GetDefaultObject()->Speed;
	}

	auto* SimpleAnimInst = Cast<USFSimpleTurretAnimInstance>(TurretComp->AnimScriptInstance);
	if (SimpleAnimInst)
	{
		SimpleAnimInst->Initialize(MuzzleName, BarrelName);
	}
	if (AimVisualizationTemplate)
	{
		FTransform Transform = GetMuzzleTransform();
		AimVisualization = GetWorld()->SpawnActorDeferred<ASFAimVisualization>(AimVisualizationTemplate, Transform);
		AimVisualization->Turret = this;
		AimVisualization->FinishSpawning(Transform);
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		AimVisualization->AttachToComponent(TurretComp, AttachmentRules, MuzzleName);
	}
}

FTransform ASFTurretActor::GetBarrelTransform() const
{
	FTransform Transform = TurretComp->GetSocketTransform(BarrelName, ERelativeTransformSpace::RTS_World);
	Transform.RemoveScaling();
	return Transform;
}

FTransform ASFTurretActor::GetMuzzleTransform() const
{
	FTransform Transform = TurretComp->GetSocketTransform(MuzzleName, ERelativeTransformSpace::RTS_World);
	Transform.RemoveScaling();
	return Transform;
}

bool ASFTurretActor::GetTarget_Implementation(ASFTurretActor* Turret, FVector& OutTarget)
{
	UObject* Delegate = DelegateRef.Get();
	if (!bActivated ||
		!Delegate ||
		!Delegate->IsValidLowLevel() ||
		!Delegate->GetClass()->ImplementsInterface(USFTurretDelegate::StaticClass()))
	{
		return false;
	}
	return ISFTurretDelegate::Execute_GetTarget(Delegate, this, OutTarget);
}

void ASFTurretActor::TriggerAction_Implementation(bool bIsPressed)
{
	if (bIsPressed)
	{
		SpawnProjectile(GetMuzzleTransform());
	}
}

//REMOVE?
float ASFTurretActor::GetTriggetAxis()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		return PC->GetInputAxisKeyValue("TriggerAxis");
	}
	return 0.0f;
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
	if (ProjectileSpeed)
	{
		Projectile->Speed = ProjectileSpeed;
	}

	UAudioComponent* AC = NULL;
	if (FireSound)
	{
		AC = UGameplayStatics::SpawnSoundAttached(FireSound, TurretComp);
	}

	UGameplayStatics::FinishSpawningActor(Projectile, Transform);

	return Projectile;
}

void ASFTurretActor::SetActivated(bool bValue)
{
	if (bValue == bActivated) return;
	ReceiveActivated(bValue);
	if (AimVisualization)
	{
		AimVisualization->SetActivated(bValue);
	}

	if (ActivationSound && bValue)
	{
		UGameplayStatics::SpawnSoundAttached(ActivationSound, TurretComp);
	}

	bActivated = bValue;
}

float ASFTurretActor::GetBarrelLength()
{
	if (auto* SimpleAnimInst = Cast<USFSimpleTurretAnimInstance>(TurretComp->AnimScriptInstance))
	{
		return SimpleAnimInst->GetBarrelLength();
	}
	return 0.0f;
}

float ASFTurretActor::GetProjectileSpeed() const
{
	return ProjectileSpeed;
}

bool ASFTurretActor::HasAimLock(float DeltaDegrees)
{
	return false;
}