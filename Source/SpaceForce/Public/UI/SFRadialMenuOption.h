#pragma once

#include "CoreMinimal.h"
#include "SFRadialMenuOption.generated.h"

USTRUCT(BlueprintType)
struct FSFRadialMenuOption
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString OptionName;

	FSFRadialMenuOption(FString OptionName) : OptionName(OptionName) {}

	FSFRadialMenuOption() : OptionName(FString("")) {}
};

