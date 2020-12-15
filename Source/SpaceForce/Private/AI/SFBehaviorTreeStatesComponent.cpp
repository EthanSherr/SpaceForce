// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AI/SFAIController.h"
#include "AI/SFSpeedParams.h"

// AIInterface
USFAIInterface::USFAIInterface(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

// BT State
USFBehaviorTreeStatesComponent::USFBehaviorTreeStatesComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USFBehaviorTreeStatesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!InitialBehavior.IsEmpty())
	{
		ChangeBehavior(InitialBehavior);
	}
}

bool USFBehaviorTreeStatesComponent::CurrentBehaviorState(FSFBehaviorTreeState& State)
{
	//Populate Defaults
	State.SpeedParams = DefaultSpeedParams;

	if (!BehaviorMap.Contains(Behavior))
	{
		return false;
	}
	State = BehaviorMap[Behavior];
	return true;
}

void USFBehaviorTreeStatesComponent::ChangeBehavior(FString NextBehavior)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn %s: BehaviorTreeStateCompnent Owner must be a Pawn"), *GetOwner()->GetName())
		return;
	}
	if (DebugDisabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn %s: ChangeBehavior failed DebugDisabled true"), *Pawn->GetName())
		return;
	}

	ASFAIController* SFController = Pawn->GetController<ASFAIController>();
	if (!SFController)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn %s: ChangeBehavior(%s) failed: No SFAIController owner."), *GetOwner()->GetName(), *NextBehavior)
		return;
	}

	if (NextBehavior == FString("_Terminate"))
	{
		SFController->UnPossess();
		if (Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass())) 
		{
			ISFAIInterface::Execute_Disable(Pawn);
		}
		return;
	}

	if (!BehaviorMap.Contains(NextBehavior))
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn %s: ChangeBehavior(%s) failed: Unknown behavior."), *GetOwner()->GetName(), *NextBehavior)
		return;
	}
	Behavior = NextBehavior;
	FSFBehaviorTreeState BehaviorState = BehaviorMap[NextBehavior];

	SFController->StartBehaviorTree(BehaviorState.BehaviorTree);
}

USFSpeedParams* USFBehaviorTreeStatesComponent::GetSpeedParams()
{
	FSFBehaviorTreeState CurrentState;
	CurrentBehaviorState(CurrentState);
	return CurrentState.SpeedParams;
}