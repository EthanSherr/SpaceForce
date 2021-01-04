#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFSpeedParams.h"
#include "SFSpeedParamsStatic.generated.h"

class AAIController;
class APawn;

UCLASS()
class SPACEFORCE_API USFSpeedParamsStatic : public USFSpeedParams
{
	GENERATED_BODY()

public:
	USFSpeedParamsStatic(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Params")
	bool bRelativeToEnemy;

	UFUNCTION()
	virtual bool Apply_Implementation(APawn* Bot, AAIController* Controller) override;
};
