// Fill out your copyright notice in the Description page of Project Settings.


#include "SFBehaviorTreePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SFAIController.h"
#include "AIController.h"
#include "../Environment/SFFlightPath.h"

ASFBehaviorTreePawn::ASFBehaviorTreePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = false;
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
	ASFAIController* SFController = GetController<ASFAIController>();
	if (!SFController)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: ChangeBehavior(%s) failed: No SFAIController owner."), *GetName(), *NextBehavior)
		return;
	}

	if (NextBehavior == FString("_Terminate")) 
	{
		SFController->UnPossess();
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

	SFController->StartBehaviorTree(BehaviorState.BehaviorTree);
	//SFController->SetFlightPathInBlackboard(BehaviorState.FlightPath);
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