// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFSplineMovementComponent.generated.h"

class USplineComponent;

UCLASS( ClassGroup="Custom", meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFSplineMovementComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Instanced)
	USplineComponent* Spline;


	//UPROPERTY(EditDefaultsOnly)
	//USplineComponent* Spline;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
