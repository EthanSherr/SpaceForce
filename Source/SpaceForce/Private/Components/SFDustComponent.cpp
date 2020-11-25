// Fill out your copyright notice in the Description page of Project Settings.


#include "SFDustComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "../Helpers/LoggingHelper.h"

USFDustComponent::USFDustComponent(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickInterval = 30 / 90.0f;

	Ground = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "Ground");
	Ground->SetupAttachment(this);

#ifdef WITH_EDITOR
	ArrowComponent = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, "Arrow Component");
	ArrowComponent->bHiddenInGame = true;
	ArrowComponent->SetupAttachment(Ground);
#endif
}

void USFDustComponent::BeginPlay()
{
	Super::BeginPlay();
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

// Called every frame
void USFDustComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const FVector Start = GetComponentLocation();
	const FVector End = Start + TraceDistance * FVector::DownVector;
	FHitResult HitResult;
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams QueryParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectParams, QueryParams))
	{
		//UE_LOG(LogTemp, Warning, TEXT("DustScan Hits: %s"), *ULoggingHelper::GetNameOrNull(HitResult.Actor.Get()));
		float DistanceFromGround = (HitResult.Location - GetComponentLocation()).Size();
		Ground->SetWorldLocation(HitResult.Location);
		if (bDebug) DrawDebugPoint(GetWorld(), Ground->GetComponentLocation(), 10, FColor::Green, false, 0, 10);
		if (!GroundEffect)
		{
			SpawnNewDustEffect(DustFX);
		}
		SetGroundDistance(DistanceFromGround);
	} 
	else
	{
		if (GroundEffect)
		{
			GroundEffect->Deactivate();
			GroundEffect = NULL;
		}
		if (bDebug) DrawDebugPoint(GetWorld(), Ground->GetComponentLocation(), 10, FColor::Red, false, 0, 10);
	}
}

void USFDustComponent::SpawnNewDustEffect(UNiagaraSystem* System)
{
	UNiagaraComponent* NiagaraComp = NewObject<UNiagaraComponent>(this);
	NiagaraComp->SetAsset(System);
	NiagaraComp->bAutoActivate = true;
	NiagaraComp->RegisterComponentWithWorld(GetWorld());
	NiagaraComp->AttachToComponent(Ground, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	GroundEffect = NiagaraComp;
}

void USFDustComponent::SetGroundDistance(float Distance)
{
	if (!GroundEffect) return;
	GroundEffect->SetFloatParameter(FName("GroundDistance"), Distance);
}

