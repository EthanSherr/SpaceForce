// Fill out your copyright notice in the Description page of Project Settings.


#include "SFAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "DrawDebugHelpers.h"
#include "Landscape.h"
#include "../Environment/SFFlightPath.h"

ASFAIController::ASFAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = false;

	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackBoardComp"));
	BrainComponent = BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
}

void ASFAIController::BeginPlay()
{
	Super::BeginPlay();
	//Disable tick!
	BrainComponent->PrimaryComponentTick.SetTickFunctionEnable(false);
	UPathFollowingComponent* PathFollowingComp = FindComponentByClass<UPathFollowingComponent>();
	if (PathFollowingComp)
	{
		PathFollowingComp->UnregisterComponent();
	}
}

void ASFAIController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
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

void ASFAIController::IsValidLocation_OLD(const FVector& Vector, bool& bIsValid, FVector& OutBumpDirection, const bool& bDebug)
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

bool ASFAIController::StartBehaviorTree(UBehaviorTree* BehaviorTree)
{
	if (BehaviorTree->BlackboardAsset) {
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}

	EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
	CanAttackKeyID = BlackboardComp->GetKeyID("CanAttack");

	BehaviorComp->PrimaryComponentTick.SetTickFunctionEnable(true);
	BehaviorComp->StartTree(*BehaviorTree);
	return true;
}