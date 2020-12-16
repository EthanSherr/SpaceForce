// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SFAIController.generated.h"

/**
 * 
 */
class ASFFlightPath;
class UBehaviorTree;

UCLASS()
class SPACEFORCE_API ASFAIController : public AAIController
{
    GENERATED_UCLASS_BODY()

protected:
    virtual void OnPossess(class APawn* InPawn) override;

    int32 EnemyKeyID;
    int32 CanAttackKeyID;

private:
    UPROPERTY(transient)
    class UBlackboardComponent* BlackboardComp;
    UPROPERTY(transient)
    class UBehaviorTreeComponent* BehaviorComp;

public:
    UFUNCTION()
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Blackboard Access")
    void SetEnemyInBlackboard(AActor* Enemy);

    UFUNCTION(BlueprintCallable, Category = "Blackboard Access")
    void SetCanAttackInBlackboard(bool InValue);

    UFUNCTION(BlueprintPure, Category = "AI Helper")
    void IsValidLocation_OLD(const FVector& Vector, bool& bIsValid, FVector& OutBumpDirection, const bool& bDebug = false);

    UFUNCTION(BlueprintCallable, Category = "AI Helper")
    void StartBehaviorTree(UBehaviorTree* BehaviorTree);

};
