#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFAttackParams.generated.h"

class AActor;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFAttackParams : public UObject
{
	GENERATED_BODY()
public:
	USFAttackParams(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInferEnemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<AActor> Enemy;
};
