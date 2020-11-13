#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SFPathParams.generated.h"

class ASFFlightPath;

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class SPACEFORCE_API USFPathParams : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASFFlightPath* FlightPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> TargetsPoints;
};
