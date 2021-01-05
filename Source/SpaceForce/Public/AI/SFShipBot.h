// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFBehaviorTreeStatesComponent.h"
#include "CoreMinimal.h"
#include "Player/SFShipPawn.h"
#include "Environment/SFTriggerableActor.h"
#include "SFShipBot.generated.h"

class ASFAIController;
class ASFPlayerTriggerBox;
class ASFTurretActor;
class USFTurretControllerManager;

UCLASS()
class SPACEFORCE_API ASFShipBot : public ASFShipPawn, public ISFAIInterface, public ISFTriggerableActor
{
	GENERATED_BODY()
	
public:
	ASFShipBot(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceivePostInitializeComponents();

public:

	UPROPERTY(EditAnywhere, Category = "Personality")
	USFBehaviorTreeStatesComponent* BehaviorStates;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Personality")
	USFTurretControllerManager* TurretManager;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFAIController* GetSFAIController() const;

	// Triggerable
	virtual void RespondToTrigger_Implementation(AActor* Source, ASFPlayerTriggerBox* TriggerBox) override;

	//AI Interface
	virtual USFBehaviorTreeStatesComponent* GetBehaviorTreeStatesComp_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetSpeed_Implementation(float Speed) override;

	UFUNCTION(BlueprintCallable)
	virtual float GetSpeed_Implementation() override;

	// Sets EnemyTracker
	UFUNCTION(BlueprintCallable)
	virtual void AttackActor_Implementation(class AActor* Actor) override;

	// Passes attack id to TurretManager
	UFUNCTION(BlueprintCallable)
	virtual void SwitchAttack_Implementation(int AttackId) override;

	UFUNCTION(BlueprintCallable)
	virtual void ReceiveDeath_Implementation(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

// SFTurretDelegate
	// Bots' turrets have TurretController delegates
	// swapped in runtime after init TurretManager->RegisterTurret(ControllerId, Turret)
	virtual bool GetTarget_Implementation(ASFTurretActor* Turret, float DeltaSeconds, FVector& OutTarget) override;
};
