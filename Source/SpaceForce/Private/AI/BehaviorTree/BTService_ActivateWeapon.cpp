#include "AI/BehaviorTree/BTService_ActivateWeapon.h"
#include "AI/BehaviorTree/Params/SFWeaponParams.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AIController.h"

UBTService_ActivateWeapon::UBTService_ActivateWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Set Active Weapon";
}

void UBTService_ActivateWeapon::HalfspaceDistance(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float Distance)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Pawn = Controller->GetPawn();

	//UE_LOG(LogTemp, Warning, TEXT("HalfspaceDistance %f"), Distance)

	if (!Pawn) return;
	if (!Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass())) return;

	auto* WeaponParams = GetWeaponParams(Pawn);
	if (!WeaponParams) return;

	const bool bInRange = (Distance >= DistanceMin && Distance <= DistanceMax);
	if (bInRange)
	{
		WeaponParams->Apply(Pawn, Controller);
	}
	else
	{
		//Clear Attacks
		ISFAIInterface::Execute_SwitchAttack(Pawn, -1);
	}
}

USFWeaponParams* UBTService_ActivateWeapon::GetWeaponParams(APawn* Pawn)
{
	if (WeaponParamsOverride)
	{
		return WeaponParamsOverride;
	}
	else
	{
		USFBehaviorTreeStatesComponent* BTStates = ISFAIInterface::Execute_GetBehaviorTreeStatesComp(Pawn);
		return BTStates->GetWeaponParams();
	}
	return NULL;
}