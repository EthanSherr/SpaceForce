// Fill out your copyright notice in the Description page of Project Settings.


#include "SFAIController.h"
#include "SFBehaviorTreePawn.h"

ASFAIController::ASFAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void ASFAIController::AttackActor(AActor* target) 
{
	ASFBehaviorTreePawn* btPawn = Cast<ASFBehaviorTreePawn>(GetPawn());
	if (btPawn) {
		btPawn->AttackActor(target);
	}
}

void ASFAIController::MoveToVector(FVector vector) {
	ASFBehaviorTreePawn* btPawn = Cast<ASFBehaviorTreePawn>(GetPawn());
	if (btPawn) {
		btPawn->MoveTo(vector);
	}
}