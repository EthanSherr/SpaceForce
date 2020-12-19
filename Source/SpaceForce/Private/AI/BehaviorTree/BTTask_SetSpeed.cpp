#include "BTTask_SetSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "../SFAIController.h"
#include "../SFSpeedParams.h"
#include "Player/SFPlayerShip.h"
#include "../../Components/SFSplineMovementComponent.h"
#include "AI/SFBehaviorTreeStatesComponent.h"

UBTTask_SetSpeed::UBTTask_SetSpeed(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Set Speed";
	bNotifyTick = false;

	EnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetSpeed, EnemyKey), AActor::StaticClass());
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

	EnemyKey.ResolveSelectedKey(*blackboard);
}

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Controller)
	{
		APawn* Pawn = Controller->GetPawn();
		if (Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
		{
			USFBehaviorTreeStatesComponent* BTStates = ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Pawn);
			USFSpeedParams* SpeedParams = BTStates->GetSpeedParams();
			if (SpeedParams)
			{
				float Speed = SpeedParams->MaxSpeed;
				if (SpeedParams->bSpeedRelativeToEnemy) {
					AActor* Enemy = Cast<AActor>(Blackboard->GetValueAsObject(EnemyKey.SelectedKeyName));
					ASFPlayerShip* ShipPawn = Cast<ASFPlayerShip>(Enemy);
					if (ShipPawn) 
					{
						USFSplineMovementComponent* SplineMovement = ShipPawn->GetAssociatedSplineMovementComponent();
						if (SplineMovement)
						{
							Speed += SplineMovement->GetMaxSpeed();
						}
					}
				}

				ISFAIInterface::Execute_SetSpeed(Pawn, Speed);

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
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
