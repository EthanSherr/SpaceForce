#include "AI/SFShipBot.h"
#include "AI/SFAIController.h"
#include "AI/SFTurretControllerManager.h"
#include "Components/SFSpringFlightMovementComponent.h"
#include "Components/SFHealthComponent.h"
#include "Components/SFTracker.h"
#include "Environment/SFPlayerTriggerBox.h"
#include "Weapons/SFTurretActor.h"
#include "Helpers/SFMath.h"

ASFShipBot::ASFShipBot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	BehaviorStates = ObjectInitializer.CreateDefaultSubobject<USFBehaviorTreeStatesComponent>(this, TEXT("BehaviorStates"));
	AIControllerClass = ASFAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TurretManager = ObjectInitializer.CreateDefaultSubobject<USFTurretControllerManager>(this, TEXT("TurretManager"));
}

void ASFShipBot::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TurretManager->Initialize(EnemyTracker);
	ReceivePostInitializeComponents();
}

void ASFShipBot::ReceiveDeath_Implementation(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Super::ReceiveDeath_Implementation(Damage, DamageEvent, EventInstigator, DamageCauser);
	//ClearAttackLoop();
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
	bool OutChanged;
	EnemyTracker->SetTargetActor(Actor, OutChanged);

	if (auto* SFController = GetSFAIController())
	{
		SFController->SetEnemyInBlackboard(Actor);
	}
}

void ASFShipBot::SwitchAttack_Implementation(int AttackId)
{
	TurretManager->SwitchController(AttackId);
}

// SFTurretDelegate & Aim
bool ASFShipBot::GetTarget_Implementation(ASFTurretActor* Turret, float DeltaSeconds, FVector& OutTarget)
{
	return false;
}