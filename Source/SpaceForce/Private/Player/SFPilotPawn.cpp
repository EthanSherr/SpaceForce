// Fill out your copyright notice in the Description page of Project Settings.


#include "SFPilotPawn.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "../Environment/SFFlightPath.h"
#include "../Components/SFSplineMovementComponent.h"
#include "../Ship/SFSpringFlightMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SFHandController.h"
#include "SpaceForce.h"
#include "SFShipPawn.h"
#include "SteamVRChaperoneComponent.h"
#include "DrawDebugHelpers.h"

ASFPilotPawn::ASFPilotPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Scene = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, FName("Root"));
	RootComponent = Scene;

	USceneComponent* VROrigin = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, FName("VROrigin"));
	VROrigin->SetupAttachment(Scene);

	Camera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, FName("Camera"));
	Camera->SetupAttachment(VROrigin);

	HandsRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, FName("HandsRoot"));
	HandsRoot->SetupAttachment(VROrigin);

	LeftHand = ObjectInitializer.CreateDefaultSubobject<USFHandController>(this, FName("LeftHand"));
	LeftHand->SetupAttachment(HandsRoot);
	LeftHand->MotionSource = FName("Left");

	RightHand = ObjectInitializer.CreateDefaultSubobject<USFHandController>(this, FName("RightHand"));
	RightHand->SetupAttachment(HandsRoot);
	RightHand->MotionSource = FName("Right");

	SplineMovement = ObjectInitializer.CreateDefaultSubobject<USFSplineMovementComponent>(this, FName("SplineMovement"));
	SplineMovement->Speed = 500.0f;

	HandExtension = 75.0f;
	VRChaperone = ObjectInitializer.CreateDefaultSubobject<USteamVRChaperoneComponent>(this, FName("VRChaperone"));

	BoostTimeline = ObjectInitializer.CreateDefaultSubobject<UTimelineComponent>(this, TEXT("BoostTimeline"));
	BoostTimelineSpeedUpdateDelegate.BindDynamic(this, &ASFPilotPawn::BoostTimelineSpeedUpdate);
	BoostTimelineFinishedDelegate.BindDynamic(this, &ASFPilotPawn::BoostTimelineFinished);

	BoostEndDrag = 0.9f;
}

void ASFPilotPawn::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property)
		return;

	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASFPilotPawn, bSpectateDebug))
	{
		if (bSpectateDebug)
		{
			AutoPossessPlayer = EAutoReceiveInput::Disabled;
			FCollisionShape MyColShape = FCollisionShape::MakeSphere(200.0f);
			TArray<FHitResult> OutHits;
			DrawDebugSphere(GetWorld(), GetActorLocation(), MyColShape.GetSphereRadius(), 100, FColor::Red, false, 0, 3.0f);
			GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_WorldStatic, MyColShape);
			for (FHitResult Hit : OutHits)
			{
				if (ASFShipPawn* HitShip = Cast<ASFShipPawn>(Hit.GetActor()))
				{
					InitializeWithShip = HitShip;
					break;
				}
			}
		} 
		else
		{
			AutoPossessPlayer = EAutoReceiveInput::Player0;
			InitializeWithShip = NULL;
		}
	}
}

void ASFPilotPawn::BeginPlay() {
	Super::BeginPlay();
	StartPilotingShip(RightHand, InitializeWithShip);
	InitializeWithShip = NULL;

	if (BoostSpeedCurve)
	{
		BoostTimeline->AddInterpFloat(BoostSpeedCurve, BoostTimelineSpeedUpdateDelegate, FName("Speed"));
		BoostTimeline->SetTimelineFinishedFunc(BoostTimelineFinishedDelegate);
	}
}

void ASFPilotPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Ship) {
		auto NextFlightPath = GetHandInState(EHandState::Driving)->GetNearestFlightPath(SplineMovement->GetFlightPath());
		SplineMovement->NextFlightPath = NextFlightPath;
	}
	auto FlightPath = SplineMovement->GetFlightPath();
	if (FlightPath) {
		FVector Tangent = FlightPath->Spline->GetDirectionAtDistanceAlongSpline(SplineMovement->GetDistance(), ESplineCoordinateSpace::World);
		HandsRoot->SetRelativeLocation(HandExtension * Tangent);
	}  
	if (!bIsBoosting && LastBoostTimelineSpeedDelta != 0.0f)
	{
		float NextBoostTimelineSpeedDelta = LastBoostTimelineSpeedDelta * BoostEndDrag;
		if (NextBoostTimelineSpeedDelta < 0.1) 
			NextBoostTimelineSpeedDelta = 0.0f;

		SetBoostSpeed(NextBoostTimelineSpeedDelta);
	}
}

void ASFPilotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &ASFPilotPawn::OnTriggerDownLeft);
	PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &ASFPilotPawn::OnTriggerDownRight);
	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &ASFPilotPawn::OnLeftGripDown);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &ASFPilotPawn::OnLeftGripUp);
	PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &ASFPilotPawn::OnRightGripDown);
	PlayerInputComponent->BindAction("RightGrip", IE_Released, this, &ASFPilotPawn::OnRightGripUp);
}

void ASFPilotPawn::OnTriggerDownLeft()  { OnTriggerDown(LeftHand); }
void ASFPilotPawn::OnTriggerDownRight() { OnTriggerDown(RightHand); }

void ASFPilotPawn::OnLeftGripDown()	 { OnGrip(LeftHand, true); }
void ASFPilotPawn::OnLeftGripUp()	 { OnGrip(LeftHand, false); }
void ASFPilotPawn::OnRightGripDown() { OnGrip(RightHand, true); }
void ASFPilotPawn::OnRightGripUp()	 { OnGrip(RightHand, false); }

void ASFPilotPawn::OnGrip(USFHandController* Hand, bool bIsPressed)
{
	if (Hand->GetHandState() == EHandState::Driving)
	{
		if (bIsPressed)
		{
			//UE_LOG(LogTemp, Warning, TEXT("LastBoostTimelineSpeedDelta START"))
			BoostTimeline->PlayFromStart();
			bIsBoosting = true;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("LastBoostTimelineSpeedDelta STOP"))
			BoostTimeline->Stop();
			bIsBoosting = false;
		}
	}
}

void ASFPilotPawn::OnTriggerDown(USFHandController* Hand) {
	if (Hand->RecievesInput())
	{
		bool bCapturesInput;
		Hand->OnTriggerDown(bCapturesInput);
		if (bCapturesInput)
			return;
	}
	switch (Hand->GetHandState()) {
	case EHandState::Ready:
		StartPilotingShip(Hand, Hand->GetOverlappingShip());
		break;
	case EHandState::Aiming:
		Ship->Fire();
		break;
	case EHandState::Driving:
		break;
	}
}

USFHandController* ASFPilotPawn::GetHandInState(TEnumAsByte<EHandState> HandState) {
	TArray<USFHandController*> Hands; Hands.Add(LeftHand); Hands.Add(RightHand);
	for (auto Hand : Hands) {
		if (Hand->GetHandState() == HandState) {
			return Hand;
		}
	}
	return NULL;
}

USFHandController* ASFPilotPawn::GetOtherHand(USFHandController* Hand) {
	if (!Hand) {
		return NULL;
	}
	return Hand == LeftHand ? RightHand : LeftHand;
}

void ASFPilotPawn::SetSpeed(float Speed) {
	SplineMovement->Speed = Speed;
}

void ASFPilotPawn::StartPilotingShip(USFHandController* NewDrivingHand, ASFShipPawn* NewShip) {
	if (!NewShip) {
		return;
	}
	NewDrivingHand->SetHandState(EHandState::Driving);
	NewShip->FlightMovement->SetTargetComponent(NewDrivingHand);

	auto NewAimingHand = GetOtherHand(NewDrivingHand);
	NewAimingHand->SetHandState(EHandState::Aiming);

	NewShip->SetOwner(this);
	NewShip->AimTargetComponent = NewAimingHand;
	Ship = NewShip;
	ReceiveStartPilotingShip();
}

// Booster Business
void ASFPilotPawn::SetBoostSpeed(float NewBoostSpeed)
{
	SplineMovement->Speed = SplineMovement->Speed - LastBoostTimelineSpeedDelta + NewBoostSpeed;
	LastBoostTimelineSpeedDelta = NewBoostSpeed;
	UE_LOG(LogTemp, Warning, TEXT("LastBoostTimelineSpeedDelta 500 +%f = %f"), NewBoostSpeed, SplineMovement->Speed)
}

void ASFPilotPawn::BoostTimelineSpeedUpdate(float Value)
{
	SetBoostSpeed(Value);
}

void ASFPilotPawn::BoostTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("BoostTimelineFinished"))
	if (BoostTimeline->GetPlaybackPosition() == 0.0f)
	{

	}
}
// Booster business end
