#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFBehaviorTreePawn.generated.h"

class UBehaviorTree;
class ASFFlightPath;
class UFloatCurve;

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

UCLASS()
class SPACEFORCE_API ASFBehaviorTreePawn : public APawn
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

public:	

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "Movement")
	float GetSpeed();

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
