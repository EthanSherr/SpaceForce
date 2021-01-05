#include "AI/BehaviorTree/Params/SFSpeedParamsChase.h"
#include "AI/SFBehaviorTreeStatesComponent.h"
#include "AI/SFAIController.h"
#include "Components/SFSplineMovementComponent.h"
#include "Environment/SFFlightPath.h"
#include "Player/SFPlayerShip.h"
#include "Curves/CurveFloat.h"
#include "DrawDebugHelpers.h"

USFSpeedParamsChase::USFSpeedParamsChase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bApplyAtBegin = false;
	static ConstructorHelpers::FObjectFinder<UCurveFloat> SpeedCurveObj(TEXT("/Game/Blueprints/AI/Common/SpeedParamsChaseCurve"));
	SpeedCurve = SpeedCurveObj.Object;
	OffsetDistance = 500.0f;

	bVisualizeDistance = false;
	bDebugOnScreenDistances = false;
}

bool USFSpeedParamsChase::Apply_Implementation(APawn* Bot, AAIController* Controller)
{
	ASFAIController* SFController = Cast<ASFAIController>(Controller);
	if (!Bot || !Bot->GetClass()->ImplementsInterface(USFAIInterface::StaticClass()) || !SFController) return false;
	if (!SpeedCurve) return false;

	AActor* Enemy = SFController->GetEnemyFromBlackboard();

	float Distance;
	if (!GetHalfSpaceDistance(Bot, Enemy, Distance)) return false;

	float Speed = SpeedCurve->GetFloatValue(-OffsetDistance + Distance);

	float RelativeSpeed;
	if (!AddEnemySpeed(Enemy, Speed, RelativeSpeed)) return false;

	if (bDebugOnScreenDistances)
	{
		FString Message = FString::Printf(TEXT("%s: Distance + OffsetDistance : %f + %f = %f\nSpeed, RelativeSpeed"),
			*Bot->GetName(), -OffsetDistance, Distance, (-OffsetDistance + Distance), Speed, RelativeSpeed);
		GEngine->AddOnScreenDebugMessage(1991, 0, FColor::Red, Message, false);
	}

	ISFAIInterface::Execute_SetSpeed(Bot, RelativeSpeed);
	return true;
}

bool USFSpeedParamsChase::GetHalfSpaceDistance(AActor* Bot, AActor* Enemy, float& OutHalfspaceDistance)
{
	if (!Enemy) return false;

	FVector EnemyForward = Enemy->GetActorForwardVector();
	FVector EnemyLocation = Enemy->GetActorLocation();
	FVector BotLocation = Bot->GetActorLocation();
	FVector RelativeLocation;

	bool bUsedFlightPath = false;
	if (ASFPlayerShip* PlayerEnemy = Cast<ASFPlayerShip>(Enemy))
	{
		if (auto* SplineMovement = PlayerEnemy->GetAssociatedSplineMovementComponent())
		{
			if (auto* FlightPath = SplineMovement->GetFlightPath())
			{
				BotLocation = FlightPath->FindLocationClosestToWorldLocation(BotLocation);
				EnemyLocation = FlightPath->FindLocationClosestToWorldLocation(EnemyLocation);

				RelativeLocation = BotLocation - EnemyLocation;
				bUsedFlightPath = true;
			}
		}
	}

	if (!bUsedFlightPath)
	{
		RelativeLocation = EnemyForward * FVector::DotProduct(EnemyForward, BotLocation - EnemyLocation);
	}
	
	if (bVisualizeDistance)
	{
		DebugDistanceVisually(EnemyLocation, BotLocation, Enemy, Bot);
	}
	OutHalfspaceDistance = FVector::DotProduct(RelativeLocation, EnemyForward);
	return true;
}

void USFSpeedParamsChase::DebugDistanceVisually(const FVector& EnemyLocation, const FVector& BotLoction, AActor* Enemy, AActor* Bot)
{
	FColor LineColor = FColor::White;
	float LineSize = 5.0f;
	float PointSize = 10.0f;
	uint8 DepthPriority = 1;

	DrawDebugLine(GetWorld(), EnemyLocation, Enemy->GetActorLocation(), LineColor, false, 0.0f, LineSize);
	DrawDebugLine(GetWorld(), BotLoction, Bot->GetActorLocation(), LineColor, false, 0.0f, LineSize);
	DrawDebugLine(GetWorld(), BotLoction, EnemyLocation, LineColor, false, 0.0f, LineSize);

	DrawDebugPoint(GetWorld(), EnemyLocation, PointSize, LineColor, false, 0.0f, DepthPriority);
	DrawDebugPoint(GetWorld(), BotLoction, PointSize, LineColor, false, 0.0f, DepthPriority);
}