#include "AI/SFShipBot.h"
#include "AI/SFAIController.h"
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
	EnemyTracker->SetTargetActor(Actor);
}

// SFTurretDelegate & Aim
bool ASFShipBot::GetTarget_Implementation(FVector& OutTarget)
{
	FVector Position;
	FVector Velocity;
	if (!EnemyTracker->GetTarget(Position, Velocity))
	{
		return false;
	}

	ASFTurretActor* Turret = GetActiveTurret();

	FProjectilePredictionResult Result = USFMath::ComputeProjectilePrediction(
		Position,
		Velocity,
		Turret->GetBarrelTransform().GetLocation(),
		Turret->GetProjectileSpeed(),
		Turret->GetBarrelLength()
	);

	OutTarget = Result.predictedImpact;

	return true;
}