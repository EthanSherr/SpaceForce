// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BalanceSpeedWithPlayer.generated.h"

class AAIController;
/**
 * 
 */
struct FBT_BalanceSpeedWithPlayer
{


	void Reset()
	{

	}
};

UCLASS()
class SPACEFORCE_API UBTService_BalanceSpeedWithPlayer : public UBTService
{
	GENERATED_BODY()

public:
    UBTService_BalanceSpeedWithPlayer();
    virtual uint16 GetInstanceMemorySize() const override;
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
