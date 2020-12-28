// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AI/SFAIController.h"
#include "AI/SFSpeedParams.h"
#include "AI/SFAttackParams.h"

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
		FSFBehaviorTreeState State;
		ChangeBehavior(InitialBehavior, State, NULL);
	}
}

bool USFBehaviorTreeStatesComponent::CurrentBehaviorState(FSFBehaviorTreeState& State)
{
	if (!BehaviorMap.Contains(Behavior))
	{
		return false;
	}
	State = BehaviorMap[Behavior];

	//Populate Defaults
	if (!State.SpeedParams)
	{
		State.SpeedParams = DefaultSpeedParams;
	}
	if (!State.AttackParams)
	{
		State.AttackParams = DefaultAttackParams;
	}

	return true;
}

bool USFBehaviorTreeStatesComponent::ChangeBehavior(FString NextBehavior, FSFBehaviorTreeState& OutState, AActor* EventInstigator)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn %s: BehaviorTreeStateCompnent Owner must be a Pawn"), *GetOwner()->GetName())
		return false;
	}
	if (DebugDisabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn %s: ChangeBehavior failed DebugDisabled true"), *Pawn->GetName())
		return false;
	}

	ASFAIController* SFController = Pawn->GetController<ASFAIController>();
	if (!SFController)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn %s: ChangeBehavior(%s) failed: No SFAIController owner."), *GetOwner()->GetName(), *NextBehavior)
		return false;
	}

	if (NextBehavior == FString("_Terminate"))
	{
		SFController->UnPossess();
		if (Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass())) 
		{
			ISFAIInterface::Execute_Disable(Pawn);
		}
		return false;
	}

	if (!BehaviorMap.Contains(NextBehavior))
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn %s: ChangeBehavior(%s) failed: Unknown behavior."), *GetOwner()->GetName(), *NextBehavior)
		return false;
	}

	OutState = BehaviorMap[NextBehavior];
	if (!OutState.BehaviorTree)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn %s: ChangeBehavior(%s) failed: No BehaviorTree provided"), *GetOwner()->GetName(), *NextBehavior)
		return false;
	}

	if (!SFController->StartBehaviorTree(OutState.BehaviorTree))
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn %s: ChangeBehavior(%s) failed: AIController failed to StartBehaviorTree"), *GetOwner()->GetName(), *NextBehavior)
		return false;
	}
	
	Behavior = NextBehavior;

	USFAttackParams* AttackParams = GetAttackParams();
	TWeakObjectPtr<AActor> Enemy = (!AttackParams || AttackParams->bInferEnemy) ? EventInstigator : AttackParams->Enemy;
	
	SFController->SetEnemyInBlackboard(Enemy.Get());

	if (AttackParams && AttackParams->InitialAttackId >= 0)
	{
		ISFAIInterface::Execute_SwitchAttack(Pawn, AttackParams->InitialAttackId);
		ISFAIInterface::Execute_AttackActor(Pawn, Enemy.Get());
	}

	return true;
}

USFSpeedParams* USFBehaviorTreeStatesComponent::GetSpeedParams()
{
	FSFBehaviorTreeState CurrentState;
	CurrentBehaviorState(CurrentState);
	return CurrentState.SpeedParams;
}

USFAttackParams* USFBehaviorTreeStatesComponent::GetAttackParams()
{
	FSFBehaviorTreeState CurrentState;
	CurrentBehaviorState(CurrentState);
	return CurrentState.AttackParams;
}

ASFAIController* USFBehaviorTreeStatesComponent::GetSFAIController() const
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return NULL;
	return Cast<ASFAIController>(Pawn->GetController());
}
