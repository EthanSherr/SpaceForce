// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetSpeed.h"
#include "../SFAIController.h"
#include "../SFBehaviorTreePawn.h"
#include "../SFSpeedParams.h"

UBTTask_SetSpeed::UBTTask_SetSpeed(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Set Speed";
	bNotifyTick = false;
}

#if WITH_EDITOR
FName UBTTask_SetSpeed::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveDirectlyToward.Icon");
}
#endif	// WITH_EDITOR

FString UBTTask_SetSpeed::GetStaticDescription() const
{
	FString ReturnDesc = Super::GetStaticDescription();

	//ReturnDesc += FString::Printf(TEXT("\n%s: %s \n"), *GET_MEMBER_NAME_CHECKED(UBTTask_FlyTo, FlightLocationKey).ToString(), *FlightLocationKey.SelectedKeyName.ToString());
	//ReturnDesc += FString("\nDebug Visualization:");
	//ReturnDesc += FString::Printf(TEXT("Raw Path: %d \n"), DebugParams.VisualizeRawPath);
	//ReturnDesc += FString::Printf(TEXT("Optimized Path: %d \n"), DebugParams.VisualizeOptimizedPath);

	return FString::Printf(TEXT("%s"), *ReturnDesc);
}

void UBTTask_SetSpeed::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	auto blackboard = GetBlackboardAsset();
	if (!blackboard)
		return;

//	FlightLocationKey.ResolveSelectedKey(*blackboard);
}

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		ASFBehaviorTreePawn* BTPawn = Cast<ASFBehaviorTreePawn>(Controller->GetPawn());
		if (BTPawn)
		{
			FSFBehaviorTreeState StateParams;
			BTPawn->CurrentBehaviorState(StateParams);
			if (StateParams.SpeedParams)
			{
				BTPawn->SetSpeed(StateParams.SpeedParams->MaxSpeed);
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_SetSpeed::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_SetSpeed::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	//FBT_FlyToTarget* myMemory = (FBT_FlyToTarget*)NodeMemory;

	//UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	//if (ensure(BlackboardComp) && myMemory->BBObserverDelegateHandle.IsValid())
	//	BlackboardComp->UnregisterObserver(FlightLocationKey.GetSelectedKeyID(), myMemory->BBObserverDelegateHandle);

	//myMemory->BBObserverDelegateHandle.Reset();

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
