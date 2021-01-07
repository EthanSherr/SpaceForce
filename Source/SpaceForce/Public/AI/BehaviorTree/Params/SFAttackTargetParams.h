#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/Params/SFAppliedParams.h"
#include "SFAttackTargetParams.generated.h"

class APawn;
class AAIController;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFAttackTargetParams : public USFAppliedParams
{
	GENERATED_BODY()

public:
	USFAttackTargetParams(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Params")
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Params")
	bool TargetIsTriggerEventInstigator;

	virtual bool Apply_Implementation(APawn* Bot, AAIController* Controller) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* NewTarget);
};
