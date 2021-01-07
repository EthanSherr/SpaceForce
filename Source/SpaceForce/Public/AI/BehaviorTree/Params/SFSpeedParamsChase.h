#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/Params/SFSpeedParams.h"
#include "SFSpeedParamsChase.generated.h"

class APawn;
class AAIController;
class UCurveFloat;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFSpeedParamsChase : public USFSpeedParams
{
	GENERATED_BODY()

public:
	USFSpeedParamsChase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Params")
	UCurveFloat* SpeedCurve;

	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Params")
	float OffsetDistance;

	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Debug")
	bool bVisualizeDistance;

	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Debug")
	bool bDebugOnScreenDistances;

	UFUNCTION()
	virtual bool Apply_Implementation(APawn* Bot, AAIController* Controller) override;

protected:
	bool GetHalfSpaceDistance(AActor* Bot, AActor* Enemy, float& OutHalfspaceDistance);

private:
	void DebugDistanceVisually(const FVector& EnemyLocation, const FVector& BotLoction, AActor* Enemy, AActor* Bot);
};
