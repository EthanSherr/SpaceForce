// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BalanceSpeedWithPlayer.h"
#include "SFFlightAIParams.h"
#include "AIController.h"

UBTService_BalanceSpeedWithPlayer::UBTService_BalanceSpeedWithPlayer()
{
    bCreateNodeInstance = true;
    bNotifyBecomeRelevant = true;

    //// accept only actors and vectors
    //BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_BalanceSpeedWithPlayer, BlackboardKey));

    //BlackboardKey.SelectedKeyName = FName("NameOfKey");
    //BlackboardKey.SelectedKeyType = UBlackboardKeyType_Vector::StaticClass();

    Interval = 0.5f;
    RandomDeviation = 0.f;
}

void UBTService_BalanceSpeedWithPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);
    auto Params = GetFlightParams(OwnerComp.GetAIOwner());
    if (!Params) return;


}

void UBTService_BalanceSpeedWithPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    UE_LOG(LogTemp, Warning, TEXT("BalanceSpeedWithPlayer"))
}

uint16 UBTService_BalanceSpeedWithPlayer::GetInstanceMemorySize() const
{
    return sizeof(FBT_BalanceSpeedWithPlayer);
}

USFFlightAIParams* UBTService_BalanceSpeedWithPlayer::GetFlightParams(AAIController* Controller)
{
    return Cast<USFFlightAIParams>(Controller->GetPawn()->GetComponentByClass(USFFlightAIParams::StaticClass()));
}