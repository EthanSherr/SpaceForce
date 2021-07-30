#include "SFTurretActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SFAimVisualization.h"
#include "SFProjectile.h"
#include "SFSimpleTurretAnimInstance.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

bool ASFTurretActor::GetTarget_Implementation(ASFTurretActor* Turret, float DeltaSeconds, FVector& OutTarget)
{
	UObject* Delegate = DelegateRef.Get();
	if (!bActivated ||
		!Delegate ||
		!Delegate->IsValidLowLevel() ||
		!Delegate->GetClass()->ImplementsInterface(USFTurretDelegate::StaticClass()))
	{
		return false;
	}
	return ISFTurretDelegate::Execute_GetTarget(Delegate, this, DeltaSeconds, OutTarget);
}

bool ASFTurretActor::GetProjectileSecondsUntilImpact(float& OutSeconds)
{
	FVector Target;
	if (!GetTarget_Implementation(this, 0, Target)) return false;
	OutSeconds = (GetMuzzleTransform().GetLocation() - Target).Size() / GetProjectileSpeed();
	return true;
}

void ASFTurretActor::TriggerAction_Implementation(bool bIsPressed)
{
	if (bIsPressed)
	{
		SpawnProjectile(GetMuzzleTransform());
	}
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
	UE_LOG(LogTemp, Warning, TEXT("ProjectileSpeed %f Projectile->Speed %f"), ProjectileSpeed, Projectile->Speed)
	if (ProjectileSpeed)
	{
		Projectile->Speed = ProjectileSpeed;
	}

	UAudioComponent* AC = NULL;
	if (FireSound)
	{
		AC = UGameplayStatics::SpawnSoundAttached(FireSound, TurretComp);
	}

	if (MuzzleFlashFx)
	{
		FTransform Transform = GetMuzzleTransform();
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFlashFx,
			TurretComp,
			FName("None"), 
			Transform.GetLocation(),
			Transform.GetRotation().Rotator(), 
			EAttachLocation::KeepWorldPosition,
			true);
	}

	UGameplayStatics::FinishSpawningActor(Projectile, Transform);

	if (auto* Delegate = DelegateRef.Get())
	{
		if (Delegate->GetClass()->ImplementsInterface(USFTurretDelegate::StaticClass()))
		{
			ISFTurretDelegate::Execute_ProjectileSpawned(Delegate, this, Projectile);
		}
	}

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

bool ASFTurretActor::GetActivated() const
{
	return bActivated;
}