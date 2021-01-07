#include "AI/BehaviorTree/Params/SFSpeedParams.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "Player/SFPlayerShip.h"

USFSpeedParams::USFSpeedParams(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bApplyAtBegin = false;
}

bool USFSpeedParams::AddEnemySpeed(AActor* Enemy, float InSpeed, float& OutSpeed)
{
	if (!Enemy)
	{
		return false;
	}
	else if (ASFPlayerShip* PlayerEnemy = Cast<ASFPlayerShip>(Enemy))
	{
		if (!PlayerEnemy->GetSpeed(OutSpeed)) return false;
	}
	else if (Enemy->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
	{
		OutSpeed = ISFAIInterface::Execute_GetSpeed(Enemy);
	}

	OutSpeed += InSpeed;
	return true;
}
