// Fill out your copyright notice in the Description page of Project Settings.


#include "SFProjectile.h"
#include "SpaceForce.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

ASFProjectile::ASFProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	LifeSpanAfterImpact = 2.0f;
	InitialLifeSpan = 7.0f;
}

void ASFProjectile::PreInitializeComponents() {
	Super::PreInitializeComponents();
	MovementComp->InitialSpeed = Speed;
	MovementComp->MaxSpeed = Speed;
}

void ASFProjectile::PostInitializeComponents() {
	Super::PostInitializeComponents();
	MovementComp->OnProjectileStop.AddDynamic(this, &ASFProjectile::OnImpact);
	CollisionComp->MoveIgnoreActors = IgnoreActors;
	CollisionComp->MoveIgnoreActors.Add(GetInstigator());
	DamageType = DamageType ? DamageType : TSubclassOf<UDamageType>(UDamageType::StaticClass());
}

void ASFProjectile::BeginPlay() 
{
	Super::BeginPlay();
}

void ASFProjectile::OnImpact(const FHitResult& HitResult) {
	DisableAndDestroy();
	Explode(HitResult);
}

void ASFProjectile::DisableAndDestroy() {
	if (AudioComp->IsPlaying()) {
		AudioComp->FadeOut(0.1f, 0.f);
	}

	MovementComp->StopMovementImmediately();
	SetLifeSpan(LifeSpanAfterImpact);
}

void ASFProjectile::Explode(const FHitResult& Impact) {
	ReceiveOnExplode(Impact);
	const FVector ImpactLocation = Impact.ImpactPoint + Impact.ImpactNormal * 0.2f;
	const FTransform SpawnTransform(Impact.ImpactNormal.Rotation(), ImpactLocation);
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

	if (ExplosionDamage > 0 && ExplosionRadius > 0) {
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, ImpactLocation, ExplosionRadius, DamageType, TArray<AActor*>(), this, Controller);
	}

	if (ExplosionTemplate) {
		AActor* const EffectActor = GetWorld()->SpawnActorDeferred<AActor>(ExplosionTemplate, SpawnTransform);
		if (EffectActor) {
			//EffectActor->SurfaceHit = Impact;
			UGameplayStatics::FinishSpawningActor(EffectActor, SpawnTransform);
		}
	}

	bExploded = true;
}