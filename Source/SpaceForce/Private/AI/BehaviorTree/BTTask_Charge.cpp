#include "AI/BehaviorTree/BTTask_Charge.h"
#include "AIController.h"
#include "AI/SFShipBot.h"
#include "AI/SFTurretControllerManager.h"
#include "AI/SFTurretController.h"
#include "Player/SFShipPawn.h"
#include "Weapons/SFTurretActor.h"
#include "Weapons/SFTurretDelegate.h"

#include "Helpers/SFMath.h"
#include "Components/SFSpringFlightMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SFTracker.h"

UBTTask_Charge::UBTTask_Charge(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Charge";
	bNotifyTick = true;

	TurretId = 0;
	Distance = 50.0f;
}

#if WITH_EDITOR

FName UBTTask_Charge::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveDirectlyToward.Icon");
}

#endif	// WITH_EDITOR

uint16 UBTTask_Charge::GetInstanceMemorySize() const
{
	return sizeof(FBT_Charge);
}

void UBTTask_Charge::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	DistanceSquared = Distance * Distance;

	//auto blackboard = GetBlackboardAsset();
	//if (!blackboard)
	//	return;

	//FlightLocationKey.ResolveSelectedKey(*blackboard);
}

EBTNodeResult::Type UBTTask_Charge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Charge::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//do cleanup
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_Charge::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBT_Charge* ChargeMemory = (FBT_Charge*)NodeMemory;

	ASFShipPawn* Pawn = Cast<ASFShipPawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Pawn) { FinishLatentAbort(OwnerComp); return; }
	if (!Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass())) { FinishLatentAbort(OwnerComp); return; }

	float PawnSpeed = ISFAIInterface::Execute_GetSpeed(Pawn);

	FVector PawnTarget;
	FVector PawnTargetVelocity;
	if (!Pawn->EnemyTracker->GetTarget(PawnTarget, PawnTargetVelocity)) { FinishLatentAbort(OwnerComp); return; }

	float DistanceToTargetSquared = (PawnTarget - Pawn->GetActorLocation()).SizeSquared();
	UE_LOG(LogTemp, Warning, TEXT("DeltaSquaredDistance %f"), (DistanceToTargetSquared - DistanceSquared))
	if (DistanceToTargetSquared < DistanceSquared)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	FVector Target;
	if (!GetChargeLocationFromTurret(Pawn, TurretId, PawnTarget, PawnTargetVelocity, Target)) { FinishLatentAbort(OwnerComp); return; }

	DrawDebugPoint(GetWorld(), Target, 5, FColor::Purple, false, 0.0f, 5);

	const FVector LastInputLocation = Pawn->FlightMovement->GetLastMoveTarget(); 
	const FVector InputWorld = (PawnSpeed * DeltaSeconds * (Target - LastInputLocation).GetSafeNormal()) + LastInputLocation;

	Pawn->AddMovementInput(InputWorld - Pawn->GetActorLocation());
}

bool UBTTask_Charge::GetChargeLocationFromTurret(ASFShipPawn* Ship, int TurretIndex, FVector TargetPos, FVector TargetVelocity, FVector& OutTarget)
{
	if (!Ship) return false;
	if (!Ship->Turrets.IsValidIndex(TurretIndex)) return false;

	ASFTurretActor* Turret = Ship->Turrets[TurretIndex];
	float ProjectileSpeed = Turret->GetProjectileSpeed();
	const FVector ProjectileOrigin = Ship->GetActorLocation();

	FProjectilePredictionResult Result = USFMath::ComputeProjectilePrediction(TargetPos, TargetVelocity, ProjectileOrigin, ProjectileSpeed, 0.0f);
	OutTarget = Result.bSuccess ? Result.predictedImpact : TargetPos;

	return true;
}

bool UBTTask_Charge::GetHalfSpaceDistance(ASFShipPawn* ShipPawn, float& OutDistance)
{
	return ShipPawn->EnemyTracker->HalfSpaceDistance(ShipPawn->GetActorLocation(), OutDistance);
}