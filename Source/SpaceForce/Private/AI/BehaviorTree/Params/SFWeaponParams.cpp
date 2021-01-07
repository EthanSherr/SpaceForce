#include "AI/BehaviorTree/Params/SFWeaponParams.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AI/SFAIController.h"

USFWeaponParams::USFWeaponParams(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WeaponId = 0;
}

bool USFWeaponParams::Apply_Implementation(APawn* Bot, AAIController* Controller)
{
	if (!Bot || !Bot->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
		return false;

	ISFAIInterface::Execute_SwitchAttack(Bot, WeaponId);
	return true;
}