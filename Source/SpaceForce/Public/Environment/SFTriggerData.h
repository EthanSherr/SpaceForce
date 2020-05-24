#pragma once

#include "CoreMinimal.h"
#include "SFTriggerData.generated.h"

UENUM(BlueprintType)
enum ESFTriggerType
{
	Default, BehaviorStart, BehaviorEnd, Exit, LevelExit,
};

USTRUCT(BlueprintType)
struct SPACEFORCE_API FSFTriggerData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Target;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ESFTriggerType> Type;

	FSFTriggerData() : Target(NULL), Type(ESFTriggerType::Default) {}
};