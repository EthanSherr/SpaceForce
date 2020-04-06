// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SFBehaviorTreePawn.generated.h"

UCLASS()
class SPACEFORCE_API ASFBehaviorTreePawn : public APawn
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AttackActor(AActor* actor);
};
