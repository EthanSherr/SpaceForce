#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetSpeed.generated.h"

UCLASS()
class SPACEFORCE_API UBTService_SetSpeed : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SetSpeed(const FObjectInitializer& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
