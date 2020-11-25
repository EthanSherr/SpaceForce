// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UAudioComponent;
class UNiagaraComponent;
class ASFExplosionEffect;

UCLASS()
class SPACEFORCE_API ASFProjectile : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ProjectileOnExplode"), Category = "Projectile")
	void ReceiveOnExplode(const FHitResult& HitResult);

protected:
	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);

	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	void Explode(const FHitResult& HitResult);

	void DisableAndDestroy();

	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float LifeSpanAfterImpact;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UNiagaraComponent* ProjectileEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float PointDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float Impulse;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	TSubclassOf<UDamageType> DamageType;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Projectile", Meta = (ExposeOnSpawn = true))
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", Meta = (ExposeOnSpawn = true))
	float Speed;

private:
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<ASFExplosionEffect> ExplosionTemplate;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<class AActor> DecalTemplate;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UProjectileMovementComponent* MovementComp;

	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UAudioComponent* AudioComp;


};
