#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/BTService_HalfspaceBase.h"
#include "BTService_ActivateWeapon.generated.h"

class USFWeaponParams;

UCLASS()
class SPACEFORCE_API UBTService_ActivateWeapon : public UBTService_HalfspaceBase
{
	GENERATED_BODY()
public:
	UBTService_ActivateWeapon(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	float DistanceMin;

	UPROPERTY(EditAnywhere)
	float DistanceMax;

	UPROPERTY(EditAnywhere)
	USFWeaponParams* WeaponParamsOverride;

protected:
	virtual void HalfspaceDistance(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float Distance) override;

	USFWeaponParams* GetWeaponParams(APawn* Pawn);
};
