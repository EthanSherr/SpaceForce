#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AI/BehaviorTree/BTService_HalfspaceBase.h"
#include "BTService_SetSpeed.generated.h"

UCLASS()
class SPACEFORCE_API UBTService_SetSpeed : public UBTService_HalfspaceBase
{
	GENERATED_BODY()
	
public:
	UBTService_SetSpeed(const FObjectInitializer& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
