// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SFPlayerTriggerResponder.h"
#include "SFPlayerTriggerBox.generated.h"

UENUM(BlueprintType)
enum EEventAction 
{
	BehaviorStart, BehaviorEnd, Exit, LevelExit,
};

USTRUCT(BlueprintType)
struct SPACEFORCE_API FEventType {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	AActor* Target;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EEventAction> Status;

	FEventType() {}
};

UCLASS()
class SPACEFORCE_API ASFPlayerTriggerBox : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> PlayerTriggerResponders;

	UPROPERTY(EditAnywhere)
	TArray<FEventType> TargetData;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void PostLoad() override;
};
