// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetWeapon.generated.h"

class USFWeaponParams;

UCLASS()
class SPACEFORCE_API UBTTask_SetWeapon : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SetWeapon(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	USFWeaponParams* OverrideWeaponParams;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:
	USFWeaponParams* GetWeaponParams(APawn* Pawn) const;
};
