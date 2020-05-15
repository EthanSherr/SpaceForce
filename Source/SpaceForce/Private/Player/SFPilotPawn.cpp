// Fill out your copyright notice in the Description page of Project Settings.


#include "SFPilotPawn.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/SFSplineMovementComponent.h"
#include "../Ship/SFSpringFlightMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SFHandController.h"
#include "SpaceForce.h"
#include "SFShipPawn.h"
#include "SteamVRChaperoneComponent.h"

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
}

void ASFPilotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

void ASFPilotPawn::OnTriggerDown(USFHandController* Hand) {
	if (GetHandInState(EHandState::Driving)) {
		// fire
	} else {
		StartPilotingShip(Hand, Hand->GetOverlappingShip());
	}
}

void ASFPilotPawn::StartPilotingShip(USFHandController* Hand, ASFShipPawn* NewShip) {
	if (!NewShip) {
		return;
	}
	Hand->SetHandState(EHandState::Driving);
	GetOtherHand(Hand)->SetHandState(EHandState::Aiming);
	NewShip->FlightMovement->SetTargetComponent(Hand);
	NewShip->SetOwner(this);
	Ship = NewShip;
}

