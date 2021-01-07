#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/Params/SFAppliedParams.h"
#include "SFWeaponParams.generated.h"

class AAIController;
class APawn;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFWeaponParams : public USFAppliedParams
{
	GENERATED_BODY()

public:
	USFWeaponParams(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	int WeaponId;

	virtual bool Apply_Implementation(APawn* Bot, AAIController* Controller) override;
};
