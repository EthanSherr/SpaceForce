// Fill out your copyright notice in the Description page of Project Settings.


#include "SFBehaviorTreePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SFAIController.h"
#include "AIController.h"
#include "../Environment/SFFlightPath.h"

ASFBehaviorTreePawn::ASFBehaviorTreePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = false;

	BehaviorStates = ObjectInitializer.CreateDefaultSubobject<USFBehaviorTreeStatesComponent>(this, TEXT("BehaviorStates"));
}

void ASFBehaviorTreePawn::BeginPlay() {
	Super::BeginPlay();
}

void ASFBehaviorTreePawn::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITORONLY_DATA
	//if (BehaviorMap.Num() && !BehaviorStates->BehaviorMap.Num())
	//{
	//	for (auto KeyValue : BehaviorMap)
	//	{
	//		BehaviorStates->BehaviorMap.Add(KeyValue);
	//	}
	//}
#endif
}

ASFAIController* ASFBehaviorTreePawn::GetSFAIController() const {
	return Cast<ASFAIController>(GetController());
}

void ASFBehaviorTreePawn::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) {
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void ASFBehaviorTreePawn::ChangeBehavior(FString NextBehavior)
{
	BehaviorStates->ChangeBehavior(NextBehavior);
}

bool ASFBehaviorTreePawn::CurrentBehaviorState(FSFBehaviorTreeState& State)
{
	return BehaviorStates->CurrentBehaviorState(State);
}

// SFAIInterface
USFBehaviorTreeStatesComponent* ASFBehaviorTreePawn::GetBehaviorTreeStatesComp_Implementation()
{
	return BehaviorStates;
}

void ASFBehaviorTreePawn::SetSpeed_Implementation(float Speed)
{
	UE_LOG(LogTemp, Error, TEXT("SetSpeed base does nothing, override SetSpeed_Implementation"))
}

float ASFBehaviorTreePawn::GetSpeed_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("GetSpeed base does nothing, override GetSpeed_Implementation"))
	return 0;
}

void ASFBehaviorTreePawn::AttackActor_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackActor base does nothing, override AttackActor_Implementatino"))
}