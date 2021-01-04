#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFSpeedParams.generated.h"

class AIController;
class UCurveFloat;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFSpeedParams : public UObject
{
	GENERATED_BODY()
	
public:
	USFSpeedParams(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Params")
	bool bApplyAtBegin;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Apply(APawn* Bot, AAIController* Controller);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool AddEnemySpeed(AActor* Enemy, float InSpeed, float& OutSpeed);
};
