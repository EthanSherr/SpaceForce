#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/Params/SFAppliedParams.h"
#include "SFSpeedParams.generated.h"

class AAIController;
class UCurveFloat;

UCLASS()
class SPACEFORCE_API USFSpeedParams : public USFAppliedParams
{
	GENERATED_BODY()
	
public:
	USFSpeedParams(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool AddEnemySpeed(AActor* Enemy, float InSpeed, float& OutSpeed);
};
