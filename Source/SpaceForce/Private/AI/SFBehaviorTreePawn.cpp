// Fill out your copyright notice in the Description page of Project Settings.


#include "SFBehaviorTreePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SFAIController.h"
#include "AIController.h"
#include "../Environment/SFFlightPath.h"

ASFBehaviorTreePawn::ASFBehaviorTreePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
}

void ASFBehaviorTreePawn::BeginPlay() {
	Super::BeginPlay();
}

ASFAIController* ASFBehaviorTreePawn::GetSFAIController() const {
	return Cast<ASFAIController>(GetController());
}

void ASFBehaviorTreePawn::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) {
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void ASFBehaviorTreePawn::ChangeBehavior(FString NextBehavior)
{
	if (DebugDisabled) return;
	ASFAIController* Controller = GetController<ASFAIController>();
	if (!Controller)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: ChangeBehavior(%s) failed: No SFAIController owner."), *GetName(), *NextBehavior)
		return;
	}

	if (NextBehavior == FString("_Terminate")) 
	{
		Controller->UnPossess();
		ReceiveDisable();
		return;
	}

	if (!BehaviorMap.Contains(NextBehavior))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: ChangeBehavior(%s) failed: Unknown behavior."), *GetName(), *NextBehavior)
		return;
	}
	Behavior = NextBehavior;
	FSFBehaviorTreeState BehaviorState = BehaviorMap[NextBehavior];

	Controller->StartBehaviorTree(BehaviorState.BehaviorTree);
	Controller->SetFlightPathInBlackboard(BehaviorState.FlightPath);
	//set some parameters?
}

bool ASFBehaviorTreePawn::CurrentBehaviorState(FSFBehaviorTreeState& State)
{
	if (!BehaviorMap.Contains(Behavior))
	{
		return false;
	}
	State = BehaviorMap[Behavior];
	return true;
}

//bool ASFBehaviorTreePawn::GetTargetVelocity(AActor* TargetActor, FVector& Velocity) const
//{
//	FVector Velocity = TargetActor->GetVelocity()
//	if (Velocity == )
//}