#pragma once

#include "CoreMinimal.h"
#include "SFTransformGenerator.generated.h"

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFTransformGenerator : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFTransformGenerator
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GenerateTransform(FTransform& OutTransform);
};
