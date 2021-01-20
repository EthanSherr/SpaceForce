// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_HalfspaceBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API UBTService_HalfspaceBase : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_HalfspaceBase(const FObjectInitializer& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:
	virtual void HalfspaceDistance(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float Distance);
};
