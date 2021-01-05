#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Weapons/SFTurretDelegate.h"
#include "Weapons/SFAttachment.h"
#include "SFShipPawn.generated.h"

class USFHealthComponent;
class USFSplineMovementComponent;
class USFSpringFlightMovementComponent;
class ASFTurretActor;
class UDestructibleMesh;
class USFDestructibleComponent;
class USFDamageType;
class ASFExplosionEffect;
class UNiagaraSystem;
class UAudioComponent;
class USFTracker;

UCLASS()
class SPACEFORCE_API ASFShipPawn : public APawn, public ISFTurretDelegate
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<FSFAttachment> TurretClasses;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<ASFTurretActor*> Turrets;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShipStaticMesh;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadonly, Category = "Effects")
	UAudioComponent* EngineAudio;

	UPROPERTY(EditAnywhere, BlueprintReadonly)
	USFTracker* EnemyTracker;

	UPROPERTY(Transient, EditDefaultsOnly, Category = "Effects")
	UDestructibleMesh* DestructibleFacade;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadonly, Category = "Effects")
	TSubclassOf<ASFExplosionEffect> ExplosionEffect;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadonly, Category = "Effects")
	UNiagaraSystem* FractureSystem;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadonly, Category = "Effects")
	TSubclassOf<ASFExplosionEffect> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFSpringFlightMovementComponent* FlightMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USFHealthComponent* HealthComponent;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPossessed();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPrimitiveComponent* GetRootPrimitive() const;

	UPROPERTY(EditInstanceOnly)
	bool bDebugCollision;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USFDamageType> ImpactDamageType;

protected:
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Death")
	void ReceiveDeath(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);

	UPROPERTY(BlueprintReadonly, Category = "Death")
	bool bDeathReceived;

	virtual void PostInitializeComponents() override;


// inventory setup
private:
	void SpawnInventory();
	void AddTurret(ASFTurretActor* Turret, FName SocketName);

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

// SFTurretDelegate

	//Default implementation: Returns position of EnemyTracker's target.
	UFUNCTION(BlueprintCallable)
	virtual bool GetTarget_Implementation(ASFTurretActor* Turret, float DeltaTime, FVector& OutTarget) override;
};
