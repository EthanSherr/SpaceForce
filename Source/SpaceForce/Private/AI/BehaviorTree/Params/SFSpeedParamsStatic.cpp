#include "AI/BehaviorTree/Params/SFSpeedParamsStatic.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AI/SFAIController.h"
#include "Player/SFPlayerShip.h"

USFSpeedParamsStatic::USFSpeedParamsStatic(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Speed = 300.0f;
	bRelativeToEnemy = true;
	bApplyAtBegin = true;
}

bool USFSpeedParamsStatic::Apply_Implementation(APawn* Bot, AAIController* Controller)
{
	ASFAIController* SFController = Cast<ASFAIController>(Controller);
	if (!Bot || !Bot->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()) || !SFController)
		return false;

	AActor* Enemy = SFController->GetEnemyFromBlackboard();
	float SpeedValue = 0.0f;

	if (!bRelativeToEnemy)
	{
		SpeedValue = Speed;
	}
	else if (!AddEnemySpeed(Enemy, SpeedValue, SpeedValue))
	{
		return false;
	}

	ISFAIInterface::Execute_SetSpeed(Bot, SpeedValue);
	return true;
}