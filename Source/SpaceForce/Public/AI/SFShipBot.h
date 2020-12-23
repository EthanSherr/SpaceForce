// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFBehaviorTreeStatesComponent.h"
#include "CoreMinimal.h"
#include "Player/SFShipPawn.h"
#include "Environment/SFTriggerableActor.h"
#include "SFShipBot.generated.h"

class ASFAIController;
class ASFPlayerTriggerBox;

UCLASS()
class SPACEFORCE_API ASFShipBot : public ASFShipPawn, public ISFAIInterface, public ISFTriggerableActor
{
	GENERATED_BODY()
	
public:
	ASFShipBot(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditAnywhere)
	USFBehaviorTreeStatesComponent* BehaviorStates;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFAIController* GetSFAIController() const;

	// Triggerable
	virtual void RespondToTrigger_Implementation(AActor* Source, ASFPlayerTriggerBox* TriggerBox) override;

	//AI Interface
	virtual USFBehaviorTreeStatesComponent* GetBehaviorTreeStatesComp_Implementation() override;

	virtual void SetSpeed_Implementation(float Speed) override;

	virtual float GetSpeed_Implementation() override;

	virtual void AttackActor_Implementation(class AActor* Actor) override;

	virtual void ReceiveDeath_Implementation(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
};
