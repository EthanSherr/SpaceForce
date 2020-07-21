// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFBehaviorTreePawn.generated.h"

UCLASS()
class SPACEFORCE_API ASFBehaviorTreePawn : public APawn
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

public:	
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
	bool DebugDisabled;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AttackActor(AActor* actor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement")
	void MoveTo(FVector location);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement")
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class ASFAIController* GetSFAIController() const;
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//bool GetTargetVelocity(AActor* TargetActor, FVector& Velocity) const;

};
