#include "AI/BehaviorTree/BTTask_Count.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Count::UBTTask_Count(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxCount = 3;
	CountKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Count, CountKey));
	bNotifyTick = false;

	NodeName = "Increment Count";
}

void UBTTask_Count::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}
EBTNodeResult::Type UBTTask_Count::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Controller = OwnerComp.GetAIOwner();
	auto* Blackboard = Controller ? Controller->FindComponentByClass<UBlackboardComponent>() : NULL;
	if (!Blackboard) return EBTNodeResult::Failed;

	int Count = Blackboard->GetValueAsInt(CountKey.SelectedKeyName) + 1;
	if (Count >= MaxCount)
	{
		Blackboard->SetValueAsInt(CountKey.SelectedKeyName, 0);
		return EBTNodeResult::Failed;
	}
	Blackboard->SetValueAsInt(CountKey.SelectedKeyName, Count);
	return EBTNodeResult::Succeeded;
}