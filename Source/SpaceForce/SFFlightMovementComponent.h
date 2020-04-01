// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFFlightMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFFlightMovementComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	
	USFFlightMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float MaxRotationSpeed;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	UFUNCTION(BlueprintCallable)
	void AddInputVector(FRotator rotation);

private:
	FRotator DeltaRotation;
};
