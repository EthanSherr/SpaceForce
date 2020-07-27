#pragma once

#include "CoreMinimal.h"
#include "SFRadialMenuOption.generated.h"

USTRUCT(BlueprintType)
struct FSFRadialMenuOption
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString OptionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* ImageMat;

	FSFRadialMenuOption(FString _OptionName, UMaterialInterface* _ImageMat) : OptionName(_OptionName), ImageMat(_ImageMat) {}

	FSFRadialMenuOption() : OptionName(FString("")), ImageMat(NULL) {}
};

