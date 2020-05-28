// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DonNavigationManager.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFDynamicPrimitiveRegistration.generated.h"

class ADonNavigationManager;
class UPrimitiveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFDynamicPrimitiveRegistration : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bRegisterOnBeginPlay;

	UPROPERTY(EditAnywhere)
	float PulseInterval;

	UPROPERTY(EditAnywhere, Category = "DON")
	bool bDisableCacheUsage;

	UPROPERTY(EditAnywhere, Category = "DON")
	bool bReloadCollisionCache;
	
	UPROPERTY(EditAnywhere, Category = "DON")
	bool bUseCheapBoundsCollision;
	
	UPROPERTY(EditAnywhere, Category = "DON")
	float BoundsScaleFactor;

	UPROPERTY(EditAnywhere, Category = "DON")
	bool bDrawDebug;

	UFUNCTION(BlueprintCallable)
	void BeginDynamicPulse();

	UFUNCTION(BlueprintCallable)
	void EndDynamicPulse();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DynamicCollisionPulse();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	bool IsValid(bool bLogError = false);
private:
	void Setup();

	FTimerHandle DynamicCollisionPulseTimerHandle;

	ADonNavigationManager* DonNavigationManager;

	UPrimitiveComponent* SimulatingPrimitive;
};
