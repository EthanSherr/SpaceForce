#pragma once
#include "CoreMinimal.h"
#include "SFAttachment.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct SPACEFORCE_API FSFAttachment
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName SocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> ActorClass;
};