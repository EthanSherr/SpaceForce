// Fill out your copyright notice in the Description page of Project Settings.


#include "SFHandController.h"
#include "Components/SphereComponent.h"
#include "../Components/SFSpringFlightMovementComponent.h"
#include "../Environment/SFFlightPath.h"
#include "../UI/SFLevelDiorama.h"
#include "SFPilotPawn.h"
#include "SFPlayerShip.h"
#include "SpaceForce.h"
#include "DrawDebugHelpers.h"
#include "../UI/SFRadialMenuComponent.h"
#include "DrawDebugHelpers.h"
#include "Helpers/LoggingHelper.h"

USFHandController::USFHandController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	this->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->SetGenerateOverlapEvents(false);

	ShipScanner = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("ShipSweeper"));
	ShipScanner->SetSphereRadius(16.0f);
	ShipScanner->bHiddenInGame = false;
	ShipScanner->SetupAttachment(this);
	ShipScanner->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShipScanner->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShipScanner->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ShipScanner->SetCollisionObjectType(ECC_Pawn);

	ShipTargetComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("ShipTargetComponent"));
	ShipTargetComponent->SetupAttachment(this);

	PathScanner = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("PathScanner"));
	PathScanner->SetSphereRadius(1000.0f);
	PathScanner->SetupAttachment(this);
	PathScanner->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PathScanner->SetCollisionResponseToAllChannels(ECR_Overlap);
	PathScanner->SetCollisionResponseToChannel(COLLISION_LANDSCAPE, ECR_Ignore);
	PathScanner->SetCollisionObjectType(ECC_Pawn);

	RadialMenuComponent = ObjectInitializer.CreateDefaultSubobject<USFRadialMenuComponent>(this, TEXT("RadialMenuComponent"));
	RadialMenuComponent->SetupAttachment(this);
	RadialMenuComponent->SetRelativeLocation(FVector(0, 0, 35));

	bScanForInteractables = false;
	ScanDistance = 1000.0f;
}

void USFHandController::BeginPlay()
{
	Super::BeginPlay();
	RadialMenuComponent->HandController = this;
}

void USFHandController::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bScanForInteractables) 
	{
		FocusInteractables();
	}

	//DrawDebugLine(
	//	GetWorld(),
	//	ShipTargetComponent->GetComponentLocation(),
	//	ShipTargetComponent->GetComponentLocation() + ShipTargetComponent->GetUpVector() * 100,
	//	FColor::Blue, false, 0, 6, 2.0f);

}

void USFHandController::FocusInteractables()
{
	const FVector Start = GetComponentLocation();
	const FVector End = Start + ScanDistance * GetForwardVector();
	const auto LastFocusedActor = FocusedActor.Get();

	FCollisionQueryParams QueryParams;
	auto Pilot = GetPilot();
	if (Pilot)
		QueryParams.AddIgnoredActor(GetPilot());
	auto Ship = GetShip();
	if (Ship)
		QueryParams.AddIgnoredActor(Ship);

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		auto NewFocusedActor = Cast<ASFLevelDiorama>(HitResult.Actor);
		if (NewFocusedActor != FocusedActor.Get())
		{
			if (FocusedActor.Get()) 
			{
				Cast<ASFLevelDiorama>(FocusedActor.Get())->SetFocused(false);
				FocusedActor.Reset();
			}

			//FocusedActor->SetFocus(false);
			UE_LOG(LogTemp, Warning, TEXT("NewFocusedActor %s"), *ULoggingHelper::GetNameOrNull(NewFocusedActor))
			if (NewFocusedActor)
			{
				NewFocusedActor->SetFocused(true);
				FocusedActor = TWeakObjectPtr<AActor>(NewFocusedActor);

			}
		}
		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, 0.0f, 2, 0.5f);
	} 
	else if (FocusedActor.Get())
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.0f, 2, 0.5f);
		Cast<ASFLevelDiorama>(FocusedActor.Get())->SetFocused(false);
		FocusedActor.Reset();
	}
}

ASFFlightPath* USFHandController::GetNearestFlightPath(ASFFlightPath* PathToIgnore) {
	TArray<AActor*> Overlapping;
	PathScanner->GetOverlappingActors(Overlapping, ASFFlightPath::StaticClass());
	float MinDistance = 0.0f;
	ASFFlightPath* NearestFlightPath = NULL;
	for (AActor* Actor : Overlapping) {
		if (PathToIgnore != Actor) {
			ASFFlightPath* FlightPath = Cast<ASFFlightPath>(Actor);
			float NewDistance = FVector::Distance(this->GetComponentLocation(), FlightPath->GetActorLocation());
			if (!NearestFlightPath || NewDistance < MinDistance) {
				MinDistance = NewDistance;
				NearestFlightPath = FlightPath;
			}
		}
	}
	return NearestFlightPath;
}

ASFPilotPawn* USFHandController::GetPilot() {
	return Cast<ASFPilotPawn>(GetOwner());
}

ASFPlayerShip* USFHandController::GetShip() {
	auto Pilot = GetPilot();
	if (!Pilot) {
		return NULL;
	}
	return Cast<ASFPlayerShip>(Pilot->Ship);
}

ASFPlayerShip* USFHandController::GetOverlappingShip() {
	TArray<AActor*> Overlapping;
	ShipScanner->GetOverlappingActors(Overlapping, ASFPlayerShip::StaticClass());
	return Overlapping.Num() > 0 ? Cast<ASFPlayerShip>(Overlapping[0]) : NULL;
}

TEnumAsByte<EHandState> USFHandController::GetHandState() {
	return HandState;
}

void USFHandController::SetHandState(TEnumAsByte<EHandState> NewState) {
	HandState = NewState;
	PathScanner->SetCollisionEnabled(HandState == EHandState::Driving ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

bool USFHandController::RecievesInput() 
{
	return bScanForInteractables;
}

void USFHandController::OnTrigger(bool& OutbCapturesInput, const bool& bPressed)
{
	if (FocusedActor.Get()) {
		Cast<ASFLevelDiorama>(FocusedActor.Get())->SetSelected(true);
	}
	OutbCapturesInput = true;
}

EControllerHand USFHandController::HandTypeFromMotionSource()
{
	if (MotionSource == FName("Left"))
		return EControllerHand::Left;
	if (MotionSource == FName("Right"))
		return EControllerHand::Right;

	return EControllerHand::AnyHand;
}