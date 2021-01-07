#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFAppliedParams.generated.h"

class AAIController;
class APawn;

UCLASS()
class SPACEFORCE_API USFAppliedParams : public UObject
{
	GENERATED_BODY()
	
public:
	USFAppliedParams(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadonly, EditAnywhere, Category = "Params")
	bool bApplyAtBegin;

	UFUNCTION(BlueprintNativeEvent)
	bool Apply(APawn* Bot, AAIController* Controller);
};
