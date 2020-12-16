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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CurrentBehaviorState(FSFBehaviorTreeState& State);

	UFUNCTION(BlueprintCallable, meta = (DeprecatedFunction, DeprecationMessage = "Function has been deprecated, Please use the new function"))
	void ChangeBehavior(FString NextBehavior);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AttackActor(AActor* actor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class ASFAIController* GetSFAIController() const;

	UPROPERTY(EditAnywhere)
	USFBehaviorTreeStatesComponent* BehaviorStates;

	virtual void PostLoad() override;

	//AI Interface
	virtual USFBehaviorTreeStatesComponent* GetBehaviorTreeStatesComp_Implementation() override;

	void SetSpeed_Implementation(float Speed) override;

	float GetSpeed_Implementation() override;

	void AttackActor_Implementation(class AActor* Actor) override;

};
