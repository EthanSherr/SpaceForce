#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "SFBTDecorator_WhileAttackCountLT.generated.h"

class USFTurretController;
class AAIController;

UCLASS()
class SPACEFORCE_API USFBTDecorator_WhileAttackCountLT : public UBTDecorator
{
	GENERATED_BODY()
public:
	USFBTDecorator_WhileAttackCountLT(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	int32 MaxAttacks;

	UPROPERTY(Transient)
	int32 AttacksRemaining;

	UPROPERTY(Transient)
	bool bNodeWasActivated;

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;

	UFUNCTION()
	void LoopFinished();

#endif // WITH_EDITOR
protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	USFTurretController* GetTurretController(AAIController* Controller);

	bool Conditional() const;

	TWeakObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponentRef;
};
