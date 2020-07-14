#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "SFBoosterManagerComponent.generated.h"

class UTimelineComponent;
class UCurveFloat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEnergyChanged, float, NewEnergy, float, MaxEnergy, float, OldEnergy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoosterActivate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoosterDeactivate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFBoosterManagerComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	UCurveFloat* SpeedBoostCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Boost")
	UCurveFloat* LinearStiffnessBoostCurve;

	UPROPERTY(EditInstanceOnly, Category = "Boost")
	float SpeedBoostDecay;

	UPROPERTY(EditInstanceOnly, Category = "Boost")
	float LinearStiffnessBoostDecay;

	UPROPERTY(BlueprintReadOnly, Category = "Boost|Energy")
	float BoosterEnergy;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Boost|Energy")
	float MaximumBoosterEnergy;

	UPROPERTY(EditInstanceOnly, Category = "Boost|Energy")
	float MinimumEnergyToStartBoost;

	UPROPERTY(EditInstanceOnly, Category = "Boost|Energy")
	float BoostEnergyDecayRate;

	UPROPERTY(EditInstanceOnly, Category = "Boost|Energy")
	float BoostEnergyRegenRate;

	UFUNCTION(BlueprintCallable)
	void TrySetIsBoosting(bool bNewIsBoosting);

	UPROPERTY(BlueprintAssignable)
	FOnEnergyChanged OnBoosterEnergyChanged;

	UPROPERTY(BlueprintAssignable)
	FOnBoosterActivate OnBoosterActivate;

	UPROPERTY(BlueprintAssignable)
	FOnBoosterDeactivate OnBoosterDeactivate;
protected:

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnBoostStart(bool& bSuccess);

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnBoostStop();

	UFUNCTION()
	void SetSpeedBoostDelta(float NewSpeedBoostDelta);

	UFUNCTION()
	void SetLinearStiffnessBoostDelta(float NewLinearStiffnessBoostDelta);

	UTimelineComponent* BoostTimeline;

	UPROPERTY(Transient)
	float SpeedBoostSpeedDelta;

	UPROPERTY(Transient)
	float LinearStiffnessBoostDelta;

	UPROPERTY(Transient)
	bool bIsBoosting;

	FOnTimelineFloat SpeedBoostTimelineUpdateDelegate;
	FOnTimelineFloat LinearStiffnessBoostTimelineUpdateDelegate;
	FOnTimelineEvent BoostTimelineFinishedDelegate;

	UFUNCTION()
	void SpeedBoostTimelineUpdate(float Value);

	UFUNCTION()
	void LinearStiffnessBoostTimelineUpdate(float Value);

	UFUNCTION()
	void BoostTimelineFinished();
};
