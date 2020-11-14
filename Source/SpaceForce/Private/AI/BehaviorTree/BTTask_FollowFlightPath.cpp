#include "BTTask_FollowFlightPath.h"
#include "../SFAIController.h"
#include "../SFBehaviorTreePawn.h"
#include "../SFPathParams.h"
#include "../../Environment/SFFlightPath.h"
#include "DrawDebugHelpers.h"

UBTTask_FollowFlightPath::UBTTask_FollowFlightPath(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Follow Flight Path";
	bNotifyTick = true;
}

uint16 UBTTask_FollowFlightPath::GetInstanceMemorySize() const
{
	return sizeof(FBT_FollowFlightPath);
}

#if WITH_EDITOR
FName UBTTask_FollowFlightPath::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveDirectlyToward.Icon");
}
#endif	// WITH_EDITOR


FString UBTTask_FollowFlightPath::GetStaticDescription() const
{
	FString ReturnDesc = Super::GetStaticDescription();

	//ReturnDesc += FString::Printf(TEXT("\n%s: %s \n"), *GET_MEMBER_NAME_CHECKED(UBTTask_FlyTo, FlightLocationKey).ToString(), *FlightLocationKey.SelectedKeyName.ToString());
	//ReturnDesc += FString("\nDebug Visualization:");
	//ReturnDesc += FString::Printf(TEXT("Raw Path: %d \n"), DebugParams.VisualizeRawPath);
	//ReturnDesc += FString::Printf(TEXT("Optimized Path: %d \n"), DebugParams.VisualizeOptimizedPath);

	return FString::Printf(TEXT("%s"), *ReturnDesc);
}

void UBTTask_FollowFlightPath::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	auto blackboard = GetBlackboardAsset();
	if (!blackboard)
		return;
}

EBTNodeResult::Type UBTTask_FollowFlightPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller)
	{
		ASFBehaviorTreePawn* BTPawn = Cast<ASFBehaviorTreePawn>(Controller->GetPawn());
		if (BTPawn)
		{
			FSFBehaviorTreeState StateParams;
			if (BTPawn->CurrentBehaviorState(StateParams) && StateParams.PathParams->FlightPath)
			{
				FBT_FollowFlightPath* Memory = (FBT_FollowFlightPath*)NodeMemory;
				ASFFlightPath* FlightPath = StateParams.PathParams->FlightPath;
				
				Memory->PathParams = StateParams.PathParams;
				Memory->PathOffset = 0.f;
				Memory->Offset = BTPawn->GetActorLocation() - FlightPath->GetLocationAtDistance(0);

				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Failed;
}


void UBTTask_FollowFlightPath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FBT_FollowFlightPath* Memory = (FBT_FollowFlightPath*)NodeMemory;
	ASFBehaviorTreePawn* Pawn = Cast<ASFBehaviorTreePawn>(OwnerComp.GetAIOwner()->GetPawn());
	USFPathParams* PathParams = Memory->PathParams.Get();
	ASFFlightPath* FlightPath = PathParams->FlightPath;
	if (!Pawn || !FlightPath)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	Memory->PathOffset += Pawn->GetSpeed() * DeltaSeconds;

	if (FlightPath->OffsetExceedsLength(Memory->PathOffset))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
	FVector SplineTarget = FlightPath->GetLocationAtDistance(Memory->PathOffset);
	Pawn->AddMovementInput(Memory->Offset + SplineTarget - Pawn->GetActorLocation());
}

void UBTTask_FollowFlightPath::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	//FBT_FlyToTarget* myMemory = (FBT_FlyToTarget*)NodeMemory;

	//UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	//if (ensure(BlackboardComp) && myMemory->BBObserverDelegateHandle.IsValid())
	//	BlackboardComp->UnregisterObserver(FlightLocationKey.GetSelectedKeyID(), myMemory->BBObserverDelegateHandle);

	//myMemory->BBObserverDelegateHandle.Reset();

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
