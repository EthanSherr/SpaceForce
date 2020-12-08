// Fill out your copyright notice in the Description page of Project Settings.


#include "SFProjectile.h"
#include "SpaceForce.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "SFExplosionEffect.h"
#include "../Helpers/LoggingHelper.h"
#include "DrawDebugHelpers.h"
#include "Weapons/SFDeflectable.h"

ASFProjectile::ASFProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(1.7f);
	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);

	RootComponent = CollisionComp;

	MovementComp = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->ProjectileGravityScale = 0.f;

	Speed = 800.0f;
	MovementComp->InitialSpeed = Speed;
	MovementComp->MaxSpeed = Speed;
	AudioComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, FName("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	ProjectileEffect = ObjectInitializer.CreateDefaultSubobject<UNiagaraComponent>(this, TEXT("ProjectileEffect"));
	ProjectileEffect->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	LifeSpanAfterImpact = 2.0f;
	InitialLifeSpan = 7.0f;

	ExplosionBump = 10.0f;
}

void ASFProjectile::PreInitializeComponents() 
{
	Super::PreInitializeComponents();
	MovementComp->InitialSpeed = Speed;
	MovementComp->MaxSpeed = Speed;
}

void ASFProjectile::PostInitializeComponents() 
{
	Super::PostInitializeComponents();
	MovementComp->OnProjectileStop.AddDynamic(this, &ASFProjectile::OnImpact);
	CollisionComp->MoveIgnoreActors = IgnoreActors;
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());
	ProjectileEffect->SetAsset(ProjectileTemplate);
	DamageType = DamageType ? DamageType : TSubclassOf<UDamageType>(UDamageType::StaticClass());
}

void ASFProjectile::BeginPlay() 
{
	Super::BeginPlay();
}

void ASFProjectile::TriggerExplosion()
{
	FHitResult HitResult;
	HitResult.ImpactPoint = GetActorLocation();
	OnImpact(HitResult);
}

void ASFProjectile::OnImpact(const FHitResult& HitResult) 
{
	AActor* HitActor = HitResult.GetActor();
	if (HitActor && HitActor->GetClass()->ImplementsInterface(USFDeflectable::StaticClass()))
	{
		bool bShouldDeflect = ISFDeflectable::Execute_ShouldDeflectProjectile(HitActor, HitResult);
		if (bShouldDeflect)
		{
			UE_LOG(LogTemp, Warning, TEXT("Deflection!"))
			FRotator DeflectedRotation = FRotationMatrix::MakeFromX(HitResult.ImpactNormal).Rotator();
			CollisionComp->SetWorldRotation(DeflectedRotation, false);
			CollisionComp->SetWorldLocation(HitResult.ImpactPoint + HitResult.ImpactNormal * 100);

			MovementComp->SetUpdatedComponent(CollisionComp);
			MovementComp->Velocity = Speed * HitResult.ImpactNormal;
			MovementComp->UpdateComponentVelocity();
			return;
		}
	}
	DisableAndDestroy();
	Explode(HitResult);
}

void ASFProjectile::DisableAndDestroy() {
	if (AudioComp->IsPlaying()) {
		AudioComp->FadeOut(0.1f, 0.f);
	}

	MovementComp->StopMovementImmediately();
	SetLifeSpan(LifeSpanAfterImpact);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ASFProjectile::Explode(const FHitResult& Impact) {
	ReceiveOnExplode(Impact);
	ProjectileEffect->SetVariableBool(FName("Exploded"), true);
	//UE_LOG(LogTemp, Warning, TEXT("SFProjectile exploded %s on %s"), *GetName(), *ULoggingHelper::GetNameOrNull(Impact.GetActor()))

	const FVector BumpedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * ExplosionBump;
	
	const FTransform SpawnTransform(Impact.ImpactNormal.Rotation(), BumpedImpactLocation);
	AController* const Controller = Cast<AController>(GetOwner());

	if (Impact.GetActor()) {
		if (Impulse > 0) {
			UPrimitiveComponent* HitPrimitive = Cast<UPrimitiveComponent>(Impact.GetActor()->GetRootComponent());
			if (HitPrimitive && HitPrimitive->IsSimulatingPhysics()) {
				HitPrimitive->AddImpulseAtLocation(-Impact.ImpactNormal * Impulse, Impact.ImpactPoint, FName());
			}
		}
		if (PointDamage > 0) {
			FPointDamageEvent PointDmg(PointDamage, Impact, Impact.ImpactNormal, DamageType);
			Impact.GetActor()->TakeDamage(PointDamage, PointDmg, Controller, this);
		}
		if (DecalTemplate) {
			AActor* const DecalActor = GetWorld()->SpawnActorDeferred<AActor>(DecalTemplate, SpawnTransform);
			if (DecalActor) {
				UGameplayStatics::FinishSpawningActor(DecalActor, SpawnTransform);
				DecalActor->AttachToActor(Impact.GetActor(), FAttachmentTransformRules::KeepWorldTransform, FName());
			}
		}
	}

	if (ExplosionDamage > 0 && ExplosionOuterRadius > 0) {
		UGameplayStatics::ApplyRadialDamageWithFalloff(
			this, 
			ExplosionDamage, 
			MinimumExplosionDamage, 
			BumpedImpactLocation, 
			ExplosionInnerRadius, 
			ExplosionOuterRadius, 
			ExplosionFalloff, 
			DamageType, 
			TArray<AActor*>(),
			this, 
			Controller);

		if (bDebugExplosion)
		{
			//DrawDebugSphere(GetWorld(), BumpedImpactLocation, ExplosionInnerRadius, 8, FColor::Yellow, false, 20.0f, 4, 2.0f);
			DrawDebugSphere(GetWorld(), BumpedImpactLocation, ExplosionInnerRadius, 8, FColor::Red, false, 3.0f, 3, 2.0f);
			DrawDebugSphere(GetWorld(), BumpedImpactLocation, ExplosionOuterRadius, 8, FColor::Green, false, 3.0f, 3, 2.0f);
		}
	}

	if (ExplosionTemplate) {
		ASFExplosionEffect* const EffectActor = GetWorld()->SpawnActorDeferred<ASFExplosionEffect>(ExplosionTemplate, SpawnTransform);
		if (EffectActor) {
			//EffectActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
		}
	}

	bExploded = true;
}