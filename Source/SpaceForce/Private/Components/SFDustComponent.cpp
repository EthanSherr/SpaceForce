// Fill out your copyright notice in the Description page of Project Settings.


#include "SFDustComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

USFDustComponent::USFDustComponent(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickInterval = 30 / 90.0f;

	ArrowComponent = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, "Arrow Component");
	ArrowComponent->SetupAttachment(this);

	Ground = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "Ground");
	Ground->SetupAttachment(this);
}


// Called when the game starts
void USFDustComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USFDustComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const FVector Start = GetComponentLocation();
	const FVector End = Start + TraceDistance * FVector::DownVector;
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		Ground->SetWorldLocation(HitResult.Location);
		Intensity = (HitResult.Location - GetComponentLocation()).Size();
		if (bDebug) DrawDebugPoint(GetWorld(), Ground->GetComponentLocation(), 10, FColor::Green, false, 0, 10);
		if (!Particles)
		{
			SpawnNewDustEffect();
		}
	} 
	else
	{
		if (Particles && Particles->IsActive())
		{
			Particles->DeactivateSystem();
			Particles = NULL;
			Intensity = 0.0f;
		}
		if (bDebug) DrawDebugPoint(GetWorld(), Ground->GetComponentLocation(), 10, FColor::Red, false, 0, 10);
	}
}

void USFDustComponent::SpawnNewDustEffect()
{
	Particles = NewObject<UParticleSystemComponent>(this);
	Particles->bAutoActivate = true;
	Particles->bAutoDestroy = true;
	Particles->RegisterComponentWithWorld(GetWorld());
	Particles->AttachToComponent(Ground, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Particles->SetTemplate(DustFX);
}

