#include "BTTask_FollowFlightPath.h"
#include "../SFAIController.h"
#include "../SFPathParams.h"
#include "../../Environment/SFFlightPath.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
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
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	if (Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
	{
		FSFBehaviorTreeState StateParams;
		USFBehaviorTreeStatesComponent* BTSComp = ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Pawn);
		if (BTSComp->CurrentBehaviorState(StateParams) && StateParams.PathParams->FlightPath)
		{
			FBT_FollowFlightPath* Memory = (FBT_FollowFlightPath*)NodeMemory;
			ASFFlightPath* FlightPath = StateParams.PathParams->FlightPath;
				
			Memory->PathOffset = 0.f;
			Memory->Offset = Pawn->GetActorLocation() - FlightPath->GetLocationAtDistance(0);

			return EBTNodeResult::InProgress;
		}
	}
	
	return EBTNodeResult::Failed;
}


void UBTTask_FollowFlightPath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	FBT_FollowFlightPath* Memory = (FBT_FollowFlightPath*)NodeMemory;
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	FSFBehaviorTreeState StateParams;

	USFBehaviorTreeStatesComponent* BTSComp = ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Pawn);
	BTSComp->CurrentBehaviorState(StateParams);
	ASFFlightPath* FlightPath = StateParams.PathParams->FlightPath;

	if (!FlightPath)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	Memory->PathOffset += ISFAIInterface::Execute_GetSpeed(Pawn) * DeltaSeconds;

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
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
