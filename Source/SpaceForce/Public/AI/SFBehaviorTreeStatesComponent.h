// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFBehaviorTreeStatesComponent.generated.h"

class UBehaviorTree;
class USFSpeedParams;
class USFPathParams;
class USFAttackParams;
class ASFAIController;

USTRUCT(BlueprintType)
struct FSFBehaviorTreeState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors")
	FString NextBehavior = FString("_Terminate");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(Instanced, BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Pathing")
	USFSpeedParams* SpeedParams;

	UPROPERTY(Instanced, BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Pathing")
	USFPathParams* PathParams;

	UPROPERTY(Instanced, BlueprintReadWRite, EditAnywhere, Category = "Behaviors|Attack")
	USFAttackParams* AttackParams;
};

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFAIInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFAIInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	USFBehaviorTreeStatesComponent* GetBehaviorTreeStatesComp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void Disable();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	float GetSpeed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void AttackActor(class AActor* Enemy);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SwitchAttack(int AttackId);
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFBehaviorTreeStatesComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	UPROPERTY(EditAnywhere, Category = "Settings")
	bool DebugDisabled;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FString InitialBehavior;

	UPROPERTY(Instanced, EditAnywhere, Category = "Settings")
	USFSpeedParams* DefaultSpeedParams;

	UPROPERTY(Instanced, EditAnywhere, Category = "Settings")
	USFAttackParams* DefaultAttackParams;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<FString, FSFBehaviorTreeState> BehaviorMap;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CurrentBehaviorState(FSFBehaviorTreeState& State);

	UFUNCTION(BlueprintCallable)
	bool ChangeBehavior(FString NextBehavior, FSFBehaviorTreeState& OutState, AActor* EventInstigator);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ASFAIController* GetSFAIController() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USFSpeedParams* GetSpeedParams();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USFAttackParams* GetAttackParams();

public:
	USFBehaviorTreeStatesComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FString Behavior;
		
};
