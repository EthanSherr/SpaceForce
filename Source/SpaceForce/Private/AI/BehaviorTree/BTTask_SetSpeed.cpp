#include "BTTask_SetSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "../SFAIController.h"
#include "AI/BehaviorTree/Params/SFSpeedParams.h"
#include "Player/SFPlayerShip.h"
#include "../../Components/SFSplineMovementComponent.h"
#include "AI/SFBehaviorTreeStatesComponent.h"

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
	return FString::Printf(TEXT("%s"), *ReturnDesc);
}

EBTNodeResult::Type UBTTask_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASFAIController* Controller = Cast<ASFAIController>(OwnerComp.GetAIOwner());
	if (!Controller) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();

	if (Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
	{
		USFSpeedParams* SpeedParams = ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Pawn)->GetSpeedParams();
		SpeedParams->Apply(Pawn, Controller);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
