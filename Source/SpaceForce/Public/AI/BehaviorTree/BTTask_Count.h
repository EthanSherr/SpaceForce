// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Count.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API UBTTask_Count : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	int MaxCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DoN Navigation")
	FBlackboardKeySelector CountKey;

public:
	UBTTask_Count(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
