#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_NavigatePoints.generated.h"

class USFPathParams;

struct SPACEFORCE_API FBT_NavigatePoints
{
	int Index;
	FVector DirectionTowardsPoint;

	void Reset()
	{

	}
};

UCLASS()
class SPACEFORCE_API UBTTask_NavigatePoints : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_NavigatePoints(const FObjectInitializer& ObjectInitializer);
	
	uint16 GetInstanceMemorySize() const;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


public:
	class USFPathParams* GetPathParams(UBehaviorTreeComponent& OwnerComp);
	bool GetTargetLocation(APawn* Pawn, USFPathParams* PathParams, int& Index, FVector& OutTarget);
};
