// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UAudioComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class ASFExplosionEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileExplode, ASFProjectile*, Projectile, FHitResult, HitResult);

UCLASS()
class SPACEFORCE_API ASFProjectile : public AActor
{
	GENERATED_UCLASS_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<ASFExplosionEffect> ExplosionTemplate;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<ASFExplosionEffect> DeflectionExplosionTemplate;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	TSubclassOf<class AActor> DecalTemplate;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UNiagaraSystem* ProjectileTemplate;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = Effects, AdvancedDisplay)
	float ExplosionBump;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float LifeSpanAfterImpact;

	UPROPERTY()
	UNiagaraComponent* ProjectileEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", Meta = (ExposeOnSpawn = true))
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionOuterRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionFalloff;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float MinimumExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	bool bDebugExplosion;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float PointDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float Impulse;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	TSubclassOf<UDamageType> DamageType;
public:
	UPROPERTY(BlueprintAssignable, Category = "DialogueManager")
	FOnProjectileExplode OnProjectileExploded;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ProjectileOnExplode"), Category = "Projectile")
	void ReceiveOnExplode(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void DelayedExplosion(float DelaySeconds);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void EnableHoming(USceneComponent* HomingComponent, float HomingAccelerationMagnitude);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void DisableHoming();

	//Used to manually explode in place.
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void TriggerExplosion();

protected:
	UFUNCTION()
	void OnImpact(const FHitResult& HitResult);

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Explode(const FHitResult& HitResult);

	void DisableAndDestroy();

	UPROPERTY(Transient)
	bool bExploded;

	//deferred impulse, sometimes the actor hit is not yet simulating
	FVector DeferredImpulse;
	FVector DeferredImpulseLocation;
	UPrimitiveComponent* HitPrimitive;
	UFUNCTION()
	void ApplyDeferredImpulse();

public:
	UPROPERTY(BlueprintReadWrite, Category = "Projectile", Meta = (ExposeOnSpawn = true))
	TArray<AActor*> IgnoreActors;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadonly, Category = Projectile)
	UProjectileMovementComponent* MovementComp;

private:
	/** collisions */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly)
	FTimerHandle DelayedExplosionHandle;
};
