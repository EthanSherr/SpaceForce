// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFShipPawn.generated.h"

class USFHealthComponent;
class USFSplineMovementComponent;
class USFSpringFlightMovementComponent;
class ASFPilotPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShipPiloted);

UCLASS()
class SPACEFORCE_API ASFShipPawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
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

	UPROPERTY(EditAnywhere)
	USFHealthComponent* HealthComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void Fire();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive();

	UPROPERTY(BlueprintAssignable)
	FOnShipPiloted OnShipPiloted;

protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnDeath(float Health);

private:
	UPROPERTY()
	USFSplineMovementComponent* AssociatedSplineMovementComponent;
};
