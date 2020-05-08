// Fill out your copyright notice in the Description page of Project Settings.


#include "SFBehaviorTreePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SFAIController.h"
#include "AIController.h"

ASFBehaviorTreePawn::ASFBehaviorTreePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
}

void ASFBehaviorTreePawn::BeginPlay() {
	AAIController* aiController = Cast<AAIController>(GetController());
	if (BehaviorTree != NULL && aiController != NULL) {
		aiController->RunBehaviorTree(BehaviorTree);
	}
	Super::BeginPlay();
}

ASFAIController* ASFBehaviorTreePawn::GetSFAIController() const {
	return Cast<ASFAIController>(GetController());
}

void ASFBehaviorTreePawn::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) {
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}