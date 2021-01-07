// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AI/SFAIController.h"
#include "AI/BehaviorTree/Params/SFAttackTargetParams.h"

#include "AI/BehaviorTree/Params/SFSpeedParams.h"
#include "AI/BehaviorTree/Params/SFWeaponParams.h"

// AIInterface
USFAIInterface::USFAIInterface(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

// BT State
USFBehaviorTreeStatesComponent::USFBehaviorTreeStatesComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	//DefaultAttackParams = ObjectInitializer.CreateDefaultSubobject <USFAttackTargetParams>(this, TEXT("AttackParams"));
	//DefaultAttackParams->TargetIsTriggerEventInstigator = true;
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

bool USFBehaviorTreeStatesComponent::GetBehaviorState(const FString& BehaviorName, FSFBehaviorTreeState& State)
{
	if (!BehaviorMap.Contains(BehaviorName))
	{
		return false;
	}
	State = BehaviorMap[BehaviorName];

	//Populate Defaults
	if (!State.SpeedParams)
	{
		State.SpeedParams = DefaultSpeedParams;
	}
	if (!State.AttackParams)
	{
		State.AttackParams = DefaultAttackParams;
	}
	if (!State.WeaponParams)
	{
		State.WeaponParams = DefaultWeaponParams;
	}

	return true;
}

bool USFBehaviorTreeStatesComponent::CurrentBehaviorState(FSFBehaviorTreeState& State)
{
	return GetBehaviorState(Behavior, State);
}

bool USFBehaviorTreeStatesComponent::ChangeBehavior(FString NextBehavior, FSFBehaviorTreeState& OutState, AActor* EventInstigator)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn %s: BehaviorTreeStateCompnent must be a pawn"), *GetOwner()->GetName())
		return false;
	}

	if (!Pawn->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn %s: BehaviorTreeStateCompnent Owner must Implmenet SFAIInterface"), *GetOwner()->GetName())
		return false;
	}

	ASFAIController* SFController = Pawn->GetController<ASFAIController>();
	if (!SFController)
	{
		UE_LOG(LogTemp, Error, TEXT("Pawn %s: ChangeBehavior(%s) failed: No SFAIController owner."), *GetOwner()->GetName(), *NextBehavior)
		return false;
	}

	if (DebugDisabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn %s: ChangeBehavior failed DebugDisabled true"), *Pawn->GetName())
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

	ApplyInitialParams(Pawn, SFController);

	return true;
}

USFSpeedParams* USFBehaviorTreeStatesComponent::GetSpeedParams()
{
	FSFBehaviorTreeState CurrentState;
	CurrentBehaviorState(CurrentState);
	return CurrentState.SpeedParams;
}

USFAttackTargetParams* USFBehaviorTreeStatesComponent::GetAttackParams()
{
	FSFBehaviorTreeState CurrentState;
	CurrentBehaviorState(CurrentState);
	return CurrentState.AttackParams;
}

USFWeaponParams* USFBehaviorTreeStatesComponent::GetWeaponParams()
{
	FSFBehaviorTreeState CurrentState;
	CurrentBehaviorState(CurrentState);
	return CurrentState.WeaponParams;
}

ASFAIController* USFBehaviorTreeStatesComponent::GetSFAIController() const
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return NULL;
	return Cast<ASFAIController>(Pawn->GetController());
}


bool USFBehaviorTreeStatesComponent::ApplyInitialParams(APawn* PawnInterface, ASFAIController* AIController)
{
	FSFBehaviorTreeState CurrentState;
	if (!CurrentBehaviorState(CurrentState)) return false;

	TArray<USFAppliedParams*> Params;
	if (auto* Attack = CurrentState.AttackParams)
	{
		Params.Add(Attack);
	}
	if (auto* Weapon = CurrentState.WeaponParams)
	{
		Params.Add(Weapon);
	}
	if (auto* Speed = CurrentState.SpeedParams)
	{
		Params.Add(Speed);
	}

	for (auto* Param : Params)
	{
		if (Param->bApplyAtBegin) 
			Param->Apply(PawnInterface, AIController);
	}

	return true;
}

//USFAttackParams* AttackParams = GetAttackParams();
//TWeakObjectPtr<AActor> Enemy = (!AttackParams || AttackParams->bInferEnemy) ? EventInstigator : AttackParams->Enemy;
//
//SFController->SetEnemyInBlackboard(Enemy.Get());

//if (AttackParams)
//{
//	ISFAIInterface::Execute_AttackActor(Pawn, Enemy.Get());
//}

//USFSpeedParams* SpeedParams = GetSpeedParams();
//if (SpeedParams && SpeedParams->bApplyAtBegin)
//{
//	SpeedParams->Apply(Pawn, SFController);
//}

//USFWeaponParams* WeaponParams = GetWeaponParams();
//if (USFWeaponParams* WeaponParams = GetWeaponParams())
//{
//	WeaponParams->Apply(Pawn, SFController);
//}