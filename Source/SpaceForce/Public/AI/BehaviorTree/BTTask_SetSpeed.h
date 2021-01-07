#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetSpeed.generated.h"

class USFSpeedParams;
class APawn;

UCLASS()
class SPACEFORCE_API UBTTask_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetSpeed(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	USFSpeedParams* OverrideSpeedParams;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:
	USFSpeedParams* GetSpeedParams(APawn* Pawn) const;
};
