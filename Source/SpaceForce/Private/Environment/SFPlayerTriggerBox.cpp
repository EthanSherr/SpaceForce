// Fill out your copyright notice in the Description page of Project Settings.

#include "SFPlayerTriggerBox.h"
#include "Player/SFPlayerShip.h"
#include "DrawDebugHelpers.h"
#include "SpaceForce.h"

ASFPlayerTriggerBox::ASFPlayerTriggerBox(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	UBoxComponent* TriggerBox = CastChecked<UBoxComponent>(GetCollisionComponent());
	TriggerBox->SetBoxExtent(FVector(300, 1000, 1000));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASFPlayerTriggerBox::OnOverlapBegin);
	TriggerBox->SetCollisionProfileName(COLLISION_PROFILE_TRIGGER);
	
#if WITH_EDITORONLY_DATA
	TriggerVisComponent = ObjectInitializer.CreateDefaultSubobject<UTriggerVisComponent>(this, TEXT("TVC"));
	TriggerVisComponent->SetupAttachment(GetRootComponent());
	TriggerVisComponent->SetIsVisualizationComponent(true);
#endif
}

void ASFPlayerTriggerBox::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITORONLY_DATA
	TriggerVisComponent->SetComponentTickEnabled(false);
#endif
}

void ASFPlayerTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ASFPlayerShip>(OtherActor)) {
		return;
	}

	for (auto* Target : Targets)
	{
		if (!Target) {
			UE_LOG(LogTemp, Error, TEXT("PlayerTrigger %s contains a NULL reference as responder."), *GetName())
			return;
		} else if (!Target->GetClass()->ImplementsInterface(USFTriggerableActor::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerTrigger %s references non null target %s which doesn't implement ISFTriggerableActor"), *GetName(), *Target->GetName())
			return;
		}
		ReceiveTrigger(Target, OtherActor);
	}
}

void ASFPlayerTriggerBox::ReceiveTrigger_Implementation(AActor* Target, AActor* Source)
{
	ISFTriggerableActor::Execute_RespondToTrigger(Target, Source, this);
}

UTriggerVisComponent::UTriggerVisComponent()
{
	bTickInEditor = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UTriggerVisComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickType == ELevelTick::LEVELTICK_ViewportsOnly)
	{
		if (UWorld* World = GetWorld())
		{
			if (ASFPlayerTriggerBox* OwnerAsTriggerBox = Cast<ASFPlayerTriggerBox>(GetOwner())) 
			{
				if (OwnerAsTriggerBox->bKeepDebugDisplayOn || IsOwnerSelected())
				{
					for (auto* Target : OwnerAsTriggerBox->Targets)
					{
						if (Target)
						{
							DrawDebugLine(World, OwnerAsTriggerBox->GetActorLocation(), Target->GetActorLocation(), FColor::Red, false, 0.001f, 0, 2.0f);
						}
					}
				}
			}
		}
	}
}