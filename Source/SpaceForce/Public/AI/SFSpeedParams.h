#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFSpeedParams.generated.h"

class UCurveFloat;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFSpeedParams : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bSpeedRelativeToEnemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxSpeed = 350.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinSpeed = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxDistance = 600.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinDistance = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCurveFloat* SpeedCurve;
};
