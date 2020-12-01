// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyPath.generated.h"

class USFPathParams;

struct SPACEFORCE_API FBT_FlyPath
{
	int Index;
	FVector Start;
	FVector Direction;
	float MaxLength;
	float Length;
	USFPathParams* PathParams;

	void Reset()
	{

	}
};

UCLASS()
class SPACEFORCE_API UBTTask_FlyPath : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FlyPath(const FObjectInitializer& ObjectInitializer);
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
	virtual FString GetStaticDescription() const override;
	uint16 GetInstanceMemorySize() const;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	class USFPathParams* GetPathParams(UBehaviorTreeComponent& OwnerComp);
	bool UpdateSegment(FBT_FlyPath* FlyPath, AActor* Pawn);
};
