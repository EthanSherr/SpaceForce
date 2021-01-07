#include "AI/BehaviorTree/Params/SFAttackTargetParams.h"
#include "AI/SFBehaviorTreeStatesComponent.h"

USFAttackTargetParams::USFAttackTargetParams(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TargetIsTriggerEventInstigator = true;
}

bool USFAttackTargetParams::Apply_Implementation(APawn* Bot, AAIController* Controller)
{
	if (!Bot || !Bot->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
		return false;

	if (Target == Bot)
	{
		UE_LOG(LogTemp, Warning, TEXT("USFAttackTargetParams  for pawn %s can't set enemy self"), *Bot->GetName())
		return false;
	}

	ISFAIInterface::Execute_AttackActor(Bot, Target);

	return true;
}

void USFAttackTargetParams::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;
}
