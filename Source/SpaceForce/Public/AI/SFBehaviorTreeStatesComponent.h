// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFBehaviorTreeStatesComponent.generated.h"

class UBehaviorTree;
class USFSpeedParams;
class USFPathParams;

USTRUCT(BlueprintType)
struct FSFBehaviorTreeState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors")
	FString NextBehavior = FString("_Terminate");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Pathing")
	USFSpeedParams* SpeedParams;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Behaviors|Pathing")
	USFPathParams* PathParams;
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

	UPROPERTY(EditAnywhere, Category = "Settings")
	USFSpeedParams* DefaultSpeedParams;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TMap<FString, FSFBehaviorTreeState> BehaviorMap;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CurrentBehaviorState(FSFBehaviorTreeState& State);

	UFUNCTION(BlueprintCallable)
	void ChangeBehavior(FString NextBehavior);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USFSpeedParams* GetSpeedParams();

public:
	USFBehaviorTreeStatesComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FString Behavior;
		
};
