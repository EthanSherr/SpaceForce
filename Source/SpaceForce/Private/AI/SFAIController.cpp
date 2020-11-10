// Fill out your copyright notice in the Description page of Project Settings.


#include "SFAIController.h"
#include "SFBehaviorTreePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DrawDebugHelpers.h"
#include "Landscape.h"
#include "../Environment/SFFlightPath.h"

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
		FlightPathKeyID = BlackboardComp->GetKeyID("FlightPath");
		if (!Bot->DebugDisabled)
		{
			BehaviorComp->StartTree(*(Bot->BehaviorTree));
		}
	}
}

void ASFAIController::SetFlightPathInBlackboard(ASFFlightPath* FlightPath) 
{
	if (BlackboardComp)
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(FlightPathKeyID, FlightPath);
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

void ASFAIController::IsValidLocation(const FVector& Vector, bool& bIsValid, FVector& OutBumpDirection, const bool& bDebug)
{
	bIsValid = true;
	TArray<FHitResult> OutHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());
	GetWorld()->LineTraceMultiByChannel(OutHits, Vector + FVector::UpVector * 500, Vector, ECC_Visibility, Params);

	if (!OutHits.Num())
		return;

	for (FHitResult Hit : OutHits)
	{
		bool bPointIsBellowLandscape = Cast<ALandscape>(Hit.GetActor()) != NULL;
		if (bDebug)
		{
			DrawDebugPoint(GetWorld(), Hit.Location, 5, FColor::Red, false, 3, 10);
		}
		FString HitName = Hit.GetActor() ? Hit.GetActor()->GetName() : FString("None");
		UE_LOG(LogTemp, Warning, TEXT("ground scan hit %s is landscape %d"), *HitName, bPointIsBellowLandscape)

		if (bPointIsBellowLandscape)
		{
			const float BumpDistance = (Hit.Location - Vector).Size() + 50.0f;
			OutBumpDirection = (Hit.Location - Vector).GetSafeNormal() * BumpDistance;
			bIsValid = false;
			return;
		}
	}
}

void ASFAIController::StartBehaviorTree(UBehaviorTree* BehaviorTree)
{
	if (BehaviorTree->BlackboardAsset) {
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}

	EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
	CanAttackKeyID = BlackboardComp->GetKeyID("CanAttack");
	FlightPathKeyID = BlackboardComp->GetKeyID("FlightPath");

	BehaviorComp->StartTree(*BehaviorTree);
}