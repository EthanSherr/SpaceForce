// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SFTriggerableActor.h"
#include "SFTriggerData.h"
#include "SFPlayerTriggerBox.generated.h"

UCLASS()
class SPACEFORCE_API ASFPlayerTriggerBox : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
	TArray<FSFTriggerData> TargetData;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
