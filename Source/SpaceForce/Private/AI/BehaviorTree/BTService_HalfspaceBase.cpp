#include "AI/BehaviorTree/BTService_HalfspaceBase.h"
#include "AIController.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AI/SFShipBot.h"
#include "Components/SFTracker.h"

UBTService_HalfspaceBase::UBTService_HalfspaceBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UBTService_HalfspaceBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass())) return;

	auto* Bot = Cast<ASFShipBot>(Pawn);
	if (!Bot) return;

	FVector Position, Velocity;
	Bot->EnemyTracker->GetTarget(Position, Velocity);

	//halfspace test
	const float HSDistance = FVector::DotProduct(Bot->GetActorLocation() - Position, Velocity.GetSafeNormal());
	HalfspaceDistance(OwnerComp, NodeMemory, HSDistance);
}

void UBTService_HalfspaceBase::HalfspaceDistance(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float Distance)
{
	UE_LOG(LogTemp, Warning, TEXT("UBTService_HalfspaceBase::HalfspaceDistance has no default implementation"))

}