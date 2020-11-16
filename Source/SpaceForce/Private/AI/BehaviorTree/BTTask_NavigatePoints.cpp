#include "BTTask_NavigatePoints.h"
#include "../SFAIController.h"
#include "../SFBehaviorTreePawn.h"
#include "../SFPathParams.h"

UBTTask_NavigatePoints::UBTTask_NavigatePoints(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Navigate Points";
	bNotifyTick = true;
}

uint16 UBTTask_NavigatePoints::GetInstanceMemorySize() const
{
	return sizeof(FBT_NavigatePoints);
}

#if WITH_EDITOR
FName UBTTask_NavigatePoints::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveDirectlyToward.Icon");
}
#endif	// WITH_EDITOR

EBTNodeResult::Type UBTTask_NavigatePoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	USFPathParams* PathParams = GetPathParams(OwnerComp);
	if (!PathParams)
		return EBTNodeResult::Failed;

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	FBT_NavigatePoints* Memory = (FBT_NavigatePoints*)NodeMemory;
	Memory->Index = 0;

	FVector Target;
	if (!GetTargetLocation(Pawn, PathParams, Memory->Index, Target))
	{
		return EBTNodeResult::Failed;
	}
	Memory->DirectionTowardsPoint = Target - Pawn->GetActorLocation();
	return EBTNodeResult::InProgress;
}

FString UBTTask_NavigatePoints::GetStaticDescription() const
{
	return Super::GetStaticDescription();
}

void UBTTask_NavigatePoints::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}

void UBTTask_NavigatePoints::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	USFPathParams* PathParams = GetPathParams(OwnerComp);
	FBT_NavigatePoints* Memory = (FBT_NavigatePoints*)NodeMemory;
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	FVector PawnLocation = Pawn->GetActorLocation();
	FVector Target;

	if (!GetTargetLocation(Pawn, PathParams, Memory->Index, Target))
	{
		UE_LOG(LogTemp, Warning, TEXT("Error, no target location"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	float HalfSpaceTest = FVector::DotProduct(PawnLocation - Target, Memory->DirectionTowardsPoint);
	UE_LOG(LogTemp, Warning, TEXT("HalfSpaceTest %f"), HalfSpaceTest)
	if (HalfSpaceTest >= 0.0f)
	{
		if (!GetTargetLocation(Pawn, PathParams, ++Memory->Index, Target))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
		Memory->DirectionTowardsPoint = Target - Pawn->GetActorLocation();
	}
	Pawn->AddMovementInput(Target - PawnLocation);
}

USFPathParams* UBTTask_NavigatePoints::GetPathParams(UBehaviorTreeComponent& OwnerComp)
{
	ASFBehaviorTreePawn* BTPawn = Cast<ASFBehaviorTreePawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (BTPawn)
	{
		FSFBehaviorTreeState BTState;
		if (BTPawn->CurrentBehaviorState(BTState))
		{
			USFPathParams* PathParams = BTState.PathParams;
			if (PathParams && PathParams->TargetsPoints.Num() > 0)
			{
				return PathParams;
			}
		}
	}
	return NULL;
}

bool UBTTask_NavigatePoints::GetTargetLocation(APawn* Pawn, USFPathParams* PathParams, int& Index, FVector& OutTarget)
{
	if (Index >= PathParams->TargetsPoints.Num())
		return false;

	AActor* Target = PathParams->TargetsPoints[Index];
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTTask_NavigatePoints for pawn '%s' has a NULL in target points, at index %d"), *Pawn->GetName(), Index)
		return GetTargetLocation(Pawn, PathParams, ++Index, OutTarget);
	}

	OutTarget = Target->GetActorLocation();
	return true;
}
