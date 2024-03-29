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
#include "Weapons/SFDamageType.h"

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

	Speed = 812.0f;
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
	
	static ConstructorHelpers::FClassFinder<USFDamageType> DamageObj(TEXT("/Game/Blueprints/Weapons/BP_ImpactDamage"));
	DamageType = DamageObj.Class ? DamageObj.Class : USFDamageType::StaticClass();
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
			FRotator DeflectedRotation = FRotationMatrix::MakeFromX(HitResult.ImpactNormal).Rotator();
			CollisionComp->SetWorldRotation(DeflectedRotation, false);
			CollisionComp->SetWorldLocation(HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f);

			MovementComp->SetUpdatedComponent(CollisionComp);
			MovementComp->Velocity = Speed * HitResult.ImpactNormal;
			MovementComp->UpdateComponentVelocity();

			if (DeflectionExplosionTemplate)
			{
				FTransform SpawnTransform(HitResult.ImpactNormal.Rotation(), HitResult.ImpactPoint + HitResult.ImpactNormal * 10.0f);
				ASFExplosionEffect* const DeflectionEffect = GetWorld()->SpawnActorDeferred<ASFExplosionEffect>(DeflectionExplosionTemplate, SpawnTransform);
				UGameplayStatics::FinishSpawningActor(DeflectionEffect, SpawnTransform);
			}
			return;
		}
	}
	DisableAndDestroy();
	Explode(HitResult);
}

void ASFProjectile::DisableAndDestroy() {
	if (AudioComp->IsPlaying()) {
		AudioComp->FadeOut(0.2f, 0.f);
	}

	DisableHoming();
	MovementComp->StopMovementImmediately();
	SetLifeSpan(LifeSpanAfterImpact);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void ASFProjectile::Explode(const FHitResult& Impact) {
	ReceiveOnExplode(Impact);
	ProjectileEffect->SetVariableBool(FName("Exploded"), true);

	const FVector BumpedImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * ExplosionBump;
	
	const FTransform SpawnTransform(Impact.ImpactNormal.Rotation(), BumpedImpactLocation);
	AController* const Controller = Cast<AController>(GetOwner());

	AActor* HitActor = Impact.Actor.Get();
	if (HitActor) {
		if (PointDamage > 0) {
			FPointDamageEvent PointDmg(PointDamage, Impact, Impact.ImpactNormal, DamageType);
			HitActor->TakeDamage(PointDamage, PointDmg, Controller, this);
		}
		if (DecalTemplate) {
			AActor* const DecalActor = GetWorld()->SpawnActorDeferred<AActor>(DecalTemplate, SpawnTransform);
			if (DecalActor) {
				UGameplayStatics::FinishSpawningActor(DecalActor, SpawnTransform);
				DecalActor->AttachToActor(HitActor, FAttachmentTransformRules::KeepWorldTransform, FName());
			}
		}
		if (Impulse > 0) {
			HitPrimitive = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
			//UE_LOG(LogTemp, Warning, TEXT("%s apply impulse to %s is Sim %d amt %s"), *GetName(), *ULoggingHelper::GetNameOrNull(HitPrimitive), HitPrimitive->IsSimulatingPhysics(), *(-Impact.ImpactNormal * Impulse).ToString())
			DeferredImpulse = -Impact.ImpactNormal * Impulse;
			DeferredImpulseLocation = Impact.ImpactPoint;

			//if (HitPrimitive) {
			//	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASFProjectile::ApplyDeferredImpulse);
			//}
			if (HitPrimitive && HitPrimitive->IsSimulatingPhysics())
			ApplyDeferredImpulse();
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

	OnProjectileExploded.Broadcast(this, Impact);
}

void ASFProjectile::ApplyDeferredImpulse()
{
	//UE_LOG(LogTemp, Warning, TEXT("ApplyDeferredImpulse from projectile %s"), *GetName())
	if (HitPrimitive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ApplyDeferredImpulse()"))
		HitPrimitive->AddImpulseAtLocation(DeferredImpulse, DeferredImpulseLocation, FName());
	}
}

void ASFProjectile::DelayedExplosion(float DelaySeconds)
{
	GetWorld()->GetTimerManager().SetTimer(DelayedExplosionHandle, this, &ASFProjectile::TriggerExplosion, DelaySeconds, false);
}

void ASFProjectile::EnableHoming(USceneComponent* HomingComponent, float HomingAccelerationMagnitude)
{
	MovementComp->bIsHomingProjectile = true;
	MovementComp->HomingTargetComponent = HomingComponent;
	MovementComp->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
}

void ASFProjectile::DisableHoming()
{
	MovementComp->HomingTargetComponent = NULL;
}

void ASFProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(DelayedExplosionHandle);
}
