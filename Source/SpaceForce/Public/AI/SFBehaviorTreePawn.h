// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFBehaviorTreePawn.generated.h"

class UBehaviorTree;
class ASFFlightPath;
class UFloatCurve;

USTRUCT(BlueprintType)
struct FSFBehaviorTreeState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors")
	FString NextBehavior = FString("_Terminate");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Flight")
	ASFFlightPath* FlightPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Speed Control")
	bool bSpeedRelativeToEnemy = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Speed Control")
	float MaxSpeed = 350.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Speed Control")
	float MinSpeed = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Speed Control")
	float MaxDistance = 600.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Speed Control")
	float MinDistance = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Speed Control")
	UCurveFloat* SpeedCurve;
};

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

	UPROPERTY(EditAnywhere, Category = "Behaviors")
	TMap<FString, FSFBehaviorTreeState> BehaviorMap;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CurrentBehaviorState(FSFBehaviorTreeState& State);

	UFUNCTION(BlueprintCallable)
	void ChangeBehavior(FString NextBehavior);

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

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveDisable();
	//UFUNCTION(BlueprintCallable, BlueprintPure)
	//bool GetTargetVelocity(AActor* TargetActor, FVector& Velocity) const;

private:
	UPROPERTY()
	FString Behavior;

};
