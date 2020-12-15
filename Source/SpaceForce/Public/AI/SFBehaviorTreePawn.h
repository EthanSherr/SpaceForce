#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "./SFBehaviorTreeStatesComponent.h"
#include "SFBehaviorTreePawn.generated.h"

class ASFFlightPath;
class UFloatCurve;

UCLASS()
class SPACEFORCE_API ASFBehaviorTreePawn : public APawn, public ISFAIInterface
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

public:	

	UPROPERTY(EditAnywhere, Category = "Behaviors")
	TMap<FString, FSFBehaviorTreeState> BehaviorMap; //M

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CurrentBehaviorState(FSFBehaviorTreeState& State);

	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function"))
	void ChangeBehavior(FString NextBehavior);

	UPROPERTY(EditAnywhere)
	bool DebugDisabled; //M

	//OLD

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AttackActor(AActor* actor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement")
	void MoveTo(FVector location);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Movement", meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function"))
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure, Category = "Movement", meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function"))
	float GetSpeed();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class ASFAIController* GetSFAIController() const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function"))
	void ReceiveDisable();

	UPROPERTY(EditAnywhere)
	USFBehaviorTreeStatesComponent* BehaviorStates;

	virtual void PostLoad() override;

	//AI Interface
	virtual USFBehaviorTreeStatesComponent* GetBehaviorTreeStatesComp_Implementation() override;

private:
	UPROPERTY()
	FString Behavior;

};
