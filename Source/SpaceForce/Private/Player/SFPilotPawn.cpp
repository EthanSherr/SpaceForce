// Fill out your copyright notice in the Description page of Project Settings.


#include "SFPilotPawn.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
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
}

void ASFPilotPawn::BeginPlay() {
	Super::BeginPlay();
	StartPilotingShip(RightHand, InitializeWithShip);
	InitializeWithShip = NULL;
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
}

void ASFPilotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &ASFPilotPawn::OnTriggerDownLeft);
	PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &ASFPilotPawn::OnTriggerDownRight);
}

void ASFPilotPawn::OnTriggerDownLeft() { OnTriggerDown(LeftHand); }
void ASFPilotPawn::OnTriggerDownRight() { OnTriggerDown(RightHand); }

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

void ASFPilotPawn::OnTriggerDown(USFHandController* Hand) {
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
}

