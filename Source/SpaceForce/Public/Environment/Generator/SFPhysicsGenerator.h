#pragma once

#include "CoreMinimal.h"
#include "SFPhysicsGenerator.generated.h"

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFPhysicsGenerator : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFPhysicsGenerator
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void GeneratePhysics(FVector& OutVelocity, FVector& OutAngularVelocity);
};
