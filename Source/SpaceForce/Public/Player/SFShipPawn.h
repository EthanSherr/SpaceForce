// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFShipPawn.generated.h"

class USFHealthComponent;
class USFSplineMovementComponent;
class USFSpringFlightMovementComponent;
class USFBoosterManagerComponent;
class ASFPilotPawn;
class ASFTurretActor;

UCLASS()
class SPACEFORCE_API ASFShipPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TSubclassOf<class ASFTurretActor>> TurretClasses;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<ASFTurretActor*> Turrets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ShipStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFSpringFlightMovementComponent* FlightMovement;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFPilotPawn* GetOwnerPilot();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USFSplineMovementComponent* GetAssociatedSplineMovementComponent();

	UPROPERTY(BlueprintReadWrite)
	USceneComponent* AimTargetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFBoosterManagerComponent* BoosterManagerComponent;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	void TrySetIsBoosting(bool bNewIsBoosting);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPossessed();

protected:
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnDeath(float Health, float MaxHealth);

private:
	UPROPERTY()
	USFSplineMovementComponent* AssociatedSplineMovementComponent;


// inventory setup
private:
	void SpawnInventory();
	void AddTurret(ASFTurretActor* Turret);

protected:
	UPROPERTY(BlueprintGetter = GetActiveTurret)
	ASFTurretActor* ActiveTurret;

public:
	UFUNCTION(BlueprintGetter)
	FORCEINLINE ASFTurretActor* GetActiveTurret() { return ActiveTurret; }
public:
	UFUNCTION(BlueprintCallable)
	void ActivateTurret(int Index);

	UFUNCTION(BlueprintCallable)
	void TriggerAction(bool bIsPressed);
// inventory end
};
