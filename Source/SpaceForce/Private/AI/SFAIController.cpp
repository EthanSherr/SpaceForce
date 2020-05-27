// Fill out your copyright notice in the Description page of Project Settings.


#include "SFAIController.h"
#include "SFBehaviorTreePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

ASFAIController::ASFAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));
	BrainComponent = BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
}

void ASFAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	ASFBehaviorTreePawn* Bot = Cast<ASFBehaviorTreePawn>(InPawn);

	if (Bot && Bot->BehaviorTree) {
		if (Bot->BehaviorTree->BlackboardAsset) {
			BlackboardComp->InitializeBlackboard(*Bot->BehaviorTree->BlackboardAsset);
		}

		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		CanAttackKeyID = BlackboardComp->GetKeyID("CanAttack");
		BehaviorComp->StartTree(*(Bot->BehaviorTree));
	}
}

void ASFAIController::SetCanAttackInBlackboard(bool InValue)
{
	if (BlackboardComp)
		BlackboardComp->SetValue<UBlackboardKeyType_Bool>(CanAttackKeyID, InValue);
}

void ASFAIController::SetEnemyInBlackboard(AActor* Enemy) {
	if (BlackboardComp) {
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, Enemy);
		//SetFocus(InPawn);
	}
}

void ASFAIController::AttackActor(AActor* target) {
	ASFBehaviorTreePawn* Bot = Cast<ASFBehaviorTreePawn>(GetPawn());
	if (Bot) {
		Bot->AttackActor(target);
	}
}

void ASFAIController::MoveToVector(FVector vector) {
	ASFBehaviorTreePawn* Bot = Cast<ASFBehaviorTreePawn>(GetPawn());
	if (Bot) {
		Bot->MoveTo(vector);
	}
}