#pragma once
#include "CoreMinimal.h"
#include "SFAttachment.generated.h"

class AActor;

USTRUCT(Blueprintable)
struct SPACEFORCE_API FSFAttachment
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	FName SocketName;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AActor> Actor;
};