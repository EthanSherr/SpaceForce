// Fill out your copyright notice in the Description page of Project Settings.


#include "SFPlayerTriggerBox.h"

ASFPlayerTriggerBox::ASFPlayerTriggerBox(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(300, 1000, 1000));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASFPlayerTriggerBox::OnOverlapBegin);
}

void ASFPlayerTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (AActor* MaybeResponder : PlayerTriggerResponders)
	{
		if (MaybeResponder->GetClass()->ImplementsInterface(USFPlayerTriggerResponder::StaticClass()))
		{
			ISFPlayerTriggerResponder::Execute_PlayerEnteredRegion(MaybeResponder);
		}
	}
}

