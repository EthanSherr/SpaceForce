// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SFTriggerableActor.h"
#include "Engine/TriggerBox.h"
#include "SFPlayerTriggerBox.generated.h"

class UTriggerVisComponent;

UCLASS()
class SPACEFORCE_API ASFPlayerTriggerBox : public ATriggerBox
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Targets;

	UFUNCTION(BlueprintNativeEvent)
	void ReceiveTrigger(AActor* Target, AActor* Source);

	UFUNCTION()
	virtual void BeginPlay() override;

	virtual void PostLoad() override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#if WITH_EDITORONLY_DATA
protected:
	UPROPERTY(Transient)
	UTriggerVisComponent* TriggerVisComponent;
public:
	UPROPERTY(Transient, EditAnywhere)
	uint8 bKeepDebugDisplayOn : 1;
#endif //WITH_EDITORONLY_DATA
};

UCLASS()
class SPACEFORCE_API UTriggerVisComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UTriggerVisComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};