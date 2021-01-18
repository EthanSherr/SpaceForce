#include "AI/BehaviorTree/SFBTDecorator_WhileAttackCountLT.h"
#include "AIController.h"
#include "AI/SFShipBot.h"
#include "AI/SFTurretControllerManager.h"
#include "AI/SFTurretController.h"
#include "BehaviorTree/BTCompositeNode.h"

USFBTDecorator_WhileAttackCountLT::USFBTDecorator_WhileAttackCountLT(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "While Attack is LT";
	MaxAttacks = 3;
	bNodeWasActivated = false;

	bCreateNodeInstance = true;

	bNotifyActivation = true;
	bNotifyDeactivation = true;

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;

	FlowAbortMode = EBTFlowAbortMode::Self;
}

FString USFBTDecorator_WhileAttackCountLT::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %d loops"), *Super::GetStaticDescription(), MaxAttacks);
}

void USFBTDecorator_WhileAttackCountLT::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
	Values.Add(FString::Printf(TEXT("loops remaining: %d"), AttacksRemaining));
}

#if WITH_EDITOR
FName USFBTDecorator_WhileAttackCountLT::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.Loop.Icon");
}
#endif	// WITH_EDITOR

void USFBTDecorator_WhileAttackCountLT::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	BehaviorTreeComponentRef = &SearchData.OwnerComp;

	if (auto* TurretController = GetTurretController(SearchData.OwnerComp.GetAIOwner()))
	{
		TurretController->LoopFinished.AddDynamic(this, &USFBTDecorator_WhileAttackCountLT::LoopFinished);
		AttacksRemaining = MaxAttacks;
		bNodeWasActivated = true;
	}
}

void USFBTDecorator_WhileAttackCountLT::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	if (auto* TurretController = GetTurretController(SearchData.OwnerComp.GetAIOwner()))
	{
		TurretController->LoopFinished.RemoveDynamic(this, &USFBTDecorator_WhileAttackCountLT::LoopFinished);
		bNodeWasActivated = false;
	}

	if (Conditional() != IsInversed())
	{
		GetParentNode()->SetChildOverride(SearchData, ChildIndex);
	}
}

void USFBTDecorator_WhileAttackCountLT::LoopFinished()
{
	AttacksRemaining -= 1;
	if (!Conditional())
	{
		if (auto* OwnerComp = BehaviorTreeComponentRef.Get())
		{
			const EBTDecoratorAbortRequest RequestMode = EBTDecoratorAbortRequest::ConditionResultChanged;
			ConditionalFlowAbort(*OwnerComp, RequestMode);
		}
	}
}

bool USFBTDecorator_WhileAttackCountLT::Conditional() const
{
	return AttacksRemaining > 0;
}

bool USFBTDecorator_WhileAttackCountLT::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const bool bResult = !bNodeWasActivated || this->Conditional();
	return bResult;
}

USFTurretController* USFBTDecorator_WhileAttackCountLT::GetTurretController(AAIController* Controller)
{
	if (Controller)
	{
		if (ASFShipBot* ShipBot = Cast<ASFShipBot>(Controller->GetPawn()))
		{
			if (USFTurretControllerManager* TurretManager = ShipBot->TurretManager)
			{
				TArray<USFTurretController*> TurretControllers;
				if (TurretManager->GetControllers(TurretControllers))
				{
					return TurretControllers[0];
				}
			}
		}
	}
	return NULL;
}
