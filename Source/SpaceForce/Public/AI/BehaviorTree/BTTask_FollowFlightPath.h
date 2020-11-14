#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FollowFlightPath.generated.h"

class APawn;
class ASFFlightPath;
class ASFPathParams;
class USFPathParams;

struct SPACEFORCE_API FBT_FollowFlightPath
{
	TWeakObjectPtr<USFPathParams> PathParams;

	FVector Offset;
	float PathOffset;

	void Reset()
	{

	}
};

UCLASS()
class SPACEFORCE_API UBTTask_FollowFlightPath : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FollowFlightPath(const FObjectInitializer& ObjectInitializer);
	
	uint16 GetInstanceMemorySize() const;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

};
