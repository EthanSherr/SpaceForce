// Fill out your copyright notice in the Description page of Project Settings.

#include "SFPlayerTriggerBox.h"
#include "../Player/SFShipPawn.h"
#include "SpaceForce.h"

ASFPlayerTriggerBox::ASFPlayerTriggerBox(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(300, 1000, 1000));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASFPlayerTriggerBox::OnOverlapBegin);
	TriggerBox->SetCollisionProfileName(COLLISION_PROFILE_TRIGGER);
}

void ASFPlayerTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* ShipPawn = Cast<ASFShipPawn>(OtherActor);
	if (!ShipPawn || !ShipPawn->GetOwnerPilot()) {
		return;
	}

	for (FSFTriggerData& Data : TargetData)
	{
		auto* MaybeResponder = Data.Target;
		if (!MaybeResponder) {
			UE_LOG(LogTemp, Error, TEXT("PlayerTrigger %s contains a NULL reference as responder."), *GetName())
		} else if (MaybeResponder->GetClass()->ImplementsInterface(USFTriggerableActor::StaticClass()))
		{
			ISFTriggerableActor::Execute_RespondToTrigger(MaybeResponder, OtherActor, Data);
		}
	}
}

