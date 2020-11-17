#include "SFDynamicPrimitiveRegistration.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "DonNavigationHelper.h"

USFDynamicPrimitiveRegistration::USFDynamicPrimitiveRegistration(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PulseInterval = 0.5f;
	bRegisterOnBeginPlay = true;
	bDisableCacheUsage = false;
	bReloadCollisionCache = false;
	bUseCheapBoundsCollision = true;
	BoundsScaleFactor = 0.0f;
	bDrawDebug = false;
}

void USFDynamicPrimitiveRegistration::BeginPlay()
{
	Super::BeginPlay();

	Setup();

	if (bRegisterOnBeginPlay)
	{
		BeginDynamicPulse();
	}
}

void USFDynamicPrimitiveRegistration::Setup()
{
	DonNavigationManager = UDonNavigationHelper::DonNavigationManagerForActor(GetOwner());
	if (!DonNavigationManager)
		return;

	if (AActor* Actor = GetOwner())
	{
		if (USceneComponent* Root = Actor->GetRootComponent())
		{
			if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(Root))
			{
				if (Primitive->IsSimulatingPhysics())
				{
					SimulatingPrimitive = Primitive;
				}
			}
		}
	}
}

bool USFDynamicPrimitiveRegistration::IsValid(bool bLogError)
{
	FString ErrorMessage;
	if (!DonNavigationManager)
	{
		ErrorMessage = "DonNavigationManager is invalid.";
	} else if (!SimulatingPrimitive)
	{
		ErrorMessage = "SimulatingPrimitive is invalid.";
	}
	bool bIsValid = ErrorMessage == FString();
	if (bLogError && !bIsValid) {
		UE_LOG(LogTemp, Error, TEXT("SFDynamicPrimitiveRegistration is not valid for actor %s: %s"), *GetOwner()->GetName(), *ErrorMessage)
	}
	return bIsValid;
}

void USFDynamicPrimitiveRegistration::BeginDynamicPulse()
{
	GetWorld()->GetTimerManager().SetTimer(DynamicCollisionPulseTimerHandle, this, &USFDynamicPrimitiveRegistration::DynamicCollisionPulse, PulseInterval, true);
}

void USFDynamicPrimitiveRegistration::EndDynamicPulse()
{
	GetWorld()->GetTimerManager().ClearTimer(DynamicCollisionPulseTimerHandle);
}

void USFDynamicPrimitiveRegistration::DynamicCollisionPulse()
{
	if (!IsValid())
	{
		Setup();
		if (!IsValid(true))
			return;
	}
	FDonCollisionSamplerCallback SamplerCallback;
	DonNavigationManager->ScheduleDynamicCollisionUpdate(SimulatingPrimitive, SamplerCallback, FName("1"), false, bDisableCacheUsage, bReloadCollisionCache, bUseCheapBoundsCollision, BoundsScaleFactor, false, bDrawDebug);
}

void USFDynamicPrimitiveRegistration::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndDynamicPulse();
	Super::EndPlay(EndPlayReason);
}