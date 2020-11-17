// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DustComponent.generated.h"

class UArrowComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API UDustComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UDustComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditInstanceOnly)
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere)
	float TraceDistance;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
