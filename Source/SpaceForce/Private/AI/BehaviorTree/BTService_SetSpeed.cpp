#include "AI/BehaviorTree/BTService_SetSpeed.h"
#include "AI/BehaviorTree/Params/SFSpeedParams.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AIController.h"

UBTService_SetSpeed::UBTService_SetSpeed(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "SetSpeed Service";
}

void UBTService_SetSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Bot = OwnerComp.GetAIOwner()->GetPawn();
	if (!Bot || !Bot->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
	{
		return;
	}

	auto* BTStates = ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Bot);
	USFSpeedParams* SpeedParams = BTStates->GetSpeedParams();

	if (!SpeedParams)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTService_SetSpeed Error, SpeedParams must be set on pawn %s behavior states"), *Bot->GetName())
		return;
	}
	
	SpeedParams->Apply(Bot, OwnerComp.GetAIOwner());
}