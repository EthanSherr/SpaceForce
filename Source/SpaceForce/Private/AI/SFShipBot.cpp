// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SFShipBot.h"
#include "AI/SFAIController.h"
#include "Components/SFSpringFlightMovementComponent.h"
#include "Environment/SFPlayerTriggerBox.h"
#include "Components/SFHealthComponent.h"

ASFShipBot::ASFShipBot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	BehaviorStates = ObjectInitializer.CreateDefaultSubobject<USFBehaviorTreeStatesComponent>(this, TEXT("BehaviorStates"));
	AIControllerClass = ASFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASFShipBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASFShipBot::ReceiveDeath_Implementation(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Super::ReceiveDeath_Implementation(Damage, DamageEvent, EventInstigator, DamageCauser);
	DetachFromControllerPendingDestroy();
	Destroy();
}


ASFAIController* ASFShipBot::GetSFAIController() const
{
	return Cast<ASFAIController>(GetController());
}

// SFTriggerableActor
void ASFShipBot::RespondToTrigger_Implementation(AActor* Source, ASFPlayerTriggerBox* TriggerBox)
{

}

// SFAIInterface
USFBehaviorTreeStatesComponent* ASFShipBot::GetBehaviorTreeStatesComp_Implementation()
{
	return BehaviorStates;
}

void ASFShipBot::SetSpeed_Implementation(float Speed)
{
	FlightMovement->SetSpeed(Speed);
}

float ASFShipBot::GetSpeed_Implementation()
{
	return FlightMovement->GetMaxSpeed();
}

void ASFShipBot::AttackActor_Implementation(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackActor base does nothing, override AttackActor_Implementatino"))
}