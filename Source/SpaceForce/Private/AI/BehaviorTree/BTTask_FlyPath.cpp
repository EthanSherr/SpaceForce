#include "AI/BehaviorTree/BTTask_FlyPath.h"
#include "../SFAIController.h"
#include "../SFPathParams.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "DrawDebugHelpers.h"

UBTTask_FlyPath::UBTTask_FlyPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Fly Path";
	bNotifyTick = true;
}

#if WITH_EDITOR
FName UBTTask_FlyPath::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveDirectlyToward.Icon");
}
#endif

FString UBTTask_FlyPath::GetStaticDescription() const
{
	return Super::GetStaticDescription();
}

uint16 UBTTask_FlyPath::GetInstanceMemorySize() const
{
	return sizeof(FBT_FlyPath);
}

void UBTTask_FlyPath::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}

EBTNodeResult::Type UBTTask_FlyPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	USFPathParams* PathParams = GetPathParams(OwnerComp);
	if (!PathParams)
		return EBTNodeResult::Failed;

	FBT_FlyPath* FlyPath = (FBT_FlyPath*)NodeMemory;
	FlyPath->Index = 0;
	FlyPath->PathParams = PathParams;

	if (!UpdateSegment(FlyPath, Pawn))
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyPath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	FBT_FlyPath* FlyPath = (FBT_FlyPath*)NodeMemory;

	float NextLength = FlyPath->Length + DeltaSeconds * ISFAIInterface::Execute_GetSpeed(Pawn);
	bool bDidIndexChange = false;
	while (NextLength >= FlyPath->MaxLength)
	{
		bDidIndexChange = true;
		NextLength -= FlyPath->MaxLength;
		FlyPath->Index++;
		if (FlyPath->Index >= FlyPath->PathParams->TargetsPoints.Num() - 1)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	//when index changes, update the start/end and length of segment, or fail if invalid.
	if (bDidIndexChange && !UpdateSegment(FlyPath, Pawn))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FlyPath->Length = NextLength;
	FVector Target = FlyPath->Start + FlyPath->Direction * FlyPath->Length;

	FVector PawnLocation = Pawn->GetActorLocation();
	Pawn->AddMovementInput(Target - PawnLocation);
}

// custom functions
USFPathParams* UBTTask_FlyPath::GetPathParams(UBehaviorTreeComponent& OwnerComp)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
	{
		USFBehaviorTreeStatesComponent* BTSComp = ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Pawn);
		
		FSFBehaviorTreeState BTState;
		if (BTSComp->CurrentBehaviorState(BTState))
		{
			USFPathParams* PathParams = BTState.PathParams;
			if (PathParams && PathParams->TargetsPoints.Num() > 0)
			{
				return PathParams;
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Invalid PathParams for pawn %s"), *OwnerComp.GetAIOwner()->GetPawn()->GetName());
	return NULL;
}

bool UBTTask_FlyPath::UpdateSegment(FBT_FlyPath* FlyPath, AActor* Pawn)
{
	int Index = FlyPath->Index;
	TArray<AActor*> Targets = FlyPath->PathParams->TargetsPoints;
	
	if (!Targets[Index] || !Targets[Index + 1])
	{
		UE_LOG(LogTemp, Error, TEXT("Target actor is NULL for pawn %s for segment %d"), *Pawn->GetName(), Index)
		return false;
	}

	FVector Start = Targets[Index]->GetActorLocation();
	FVector End = Targets[Index + 1]->GetActorLocation();
	FVector Direction = End - Start;
	float MaxLength = Direction.Size();
	if (MaxLength == 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Segments are the same point at index %d for %s"), Index, *Pawn->GetName())
		return false;
	}
	Direction /= MaxLength;

	FlyPath->Start = Start;
	FlyPath->Direction = Direction;
	FlyPath->MaxLength = MaxLength;

	return true;
}