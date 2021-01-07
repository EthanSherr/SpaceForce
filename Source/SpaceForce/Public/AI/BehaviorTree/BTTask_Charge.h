#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Charge.generated.h"

class APawn;
class ASFShipPawn;

struct FBT_Charge
{

};

UCLASS()
class SPACEFORCE_API UBTTask_Charge : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	int TurretId;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Distance;
public:
	UBTTask_Charge(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

	virtual uint16 GetInstanceMemorySize() const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool GetChargeLocationFromTurret(ASFShipPawn* Ship, int TurretIndex, FVector TargetPos, FVector TargetVelocity, FVector& OutTarget);

	bool GetHalfSpaceDistance(ASFShipPawn* ShipPawn, float& OutDistance);
private:
	UPROPERTY(Transient)
	float DistanceSquared;
};
