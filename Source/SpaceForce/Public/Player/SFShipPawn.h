// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFShipPawn.generated.h"

class USFHealthComponent;
class USFSplineMovementComponent;
class USFSpringFlightMovementComponent;
class ASFTurretActor;
class UDestructibleMesh;
class USFDestructibleComponent;

UCLASS()
class SPACEFORCE_API ASFShipPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TSubclassOf<class ASFTurretActor>> TurretClasses;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<ASFTurretActor*> Turrets;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShipStaticMesh;

	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UDestructibleMesh* DestructibleFacade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFSpringFlightMovementComponent* FlightMovement;

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* AimTargetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFHealthComponent* HealthComponent;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPossessed();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPrimitiveComponent* GetRootPrimitive() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void PostInitializeComponents() override;

// SFHealthComponent delegate methods
	UFUNCTION()
	virtual void OnDeath(USFHealthComponent* HealthComp, float Damage);

	//Deals damage to DestructibleComponent using DamageEvent's USFDamageType's DestructibleDamage
	//UFUNCTION()
	//virtual void TakeDestructibleDamage(struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);


// inventory setup
private:
	void SpawnInventory();
	void AddTurret(ASFTurretActor* Turret);

	// Created after ship dies
	UPROPERTY(Transient)
	USFDestructibleComponent* DestructibleComp;

protected:
	UPROPERTY(BlueprintGetter = GetActiveTurret)
	ASFTurretActor* ActiveTurret;

public:
	UFUNCTION(BlueprintGetter)
	FORCEINLINE ASFTurretActor* GetActiveTurret() { return ActiveTurret; }
public:
	UFUNCTION(BlueprintCallable)
	ASFTurretActor* ActivateTurret(int Index);

	UFUNCTION(BlueprintCallable)
	void TriggerAction(bool bIsPressed);
// inventory end
};
