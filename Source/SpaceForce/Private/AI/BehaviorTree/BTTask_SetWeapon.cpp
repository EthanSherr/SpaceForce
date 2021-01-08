#include "AI/BehaviorTree/BTTask_SetWeapon.h"
#include "AI/BehaviorTree/Params/SFWeaponParams.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AIController.h"

UBTTask_SetWeapon::UBTTask_SetWeapon(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Set Weapon";
	bNotifyTick = false;
}

#if WITH_EDITOR
FName UBTTask_SetWeapon::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveDirectlyToward.Icon");
}
#endif	// WITH_EDITOR


FString UBTTask_SetWeapon::GetStaticDescription() const
{
	FString ReturnDesc = Super::GetStaticDescription();
	return FString::Printf(TEXT("%s"), *ReturnDesc);
}

EBTNodeResult::Type UBTTask_SetWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Pawn = Controller->GetPawn();

	if (USFWeaponParams* WeaponParams = GetWeaponParams(Pawn))
	{
		WeaponParams->Apply(Pawn, Controller);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

USFWeaponParams* UBTTask_SetWeapon::GetWeaponParams(APawn* Pawn) const
{
	if (OverrideWeaponParams)
	{
		return OverrideWeaponParams;
	}
	if (Pawn && Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
	{
		return ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Pawn)->GetWeaponParams();
	}
	return NULL;
}
