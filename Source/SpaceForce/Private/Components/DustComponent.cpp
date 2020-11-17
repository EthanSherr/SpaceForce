// Fill out your copyright notice in the Description page of Project Settings.


#include "DustComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"

UDustComponent::UDustComponent(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	ArrowComponent = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, "Arrow Component");
	ArrowComponent->SetupAttachment(this);
}


// Called when the game starts
void UDustComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UDustComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const FVector Start = GetComponentLocation();
	const FVector End = Start + TraceDistance * FVector::DownVector;
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Green, false, 0, 10);
	} 
	else
	{
		DrawDebugPoint(GetWorld(), End, 10, FColor::Red, false, 0, 10);
	}
}

