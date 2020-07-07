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
	SpeedBoostTimelineUpdateDelegate.BindDynamic(this, &ASFPilotPawn::SpeedBoostTimelineUpdate);
	LinearStiffnessBoostTimelineUpdateDelegate.BindDynamic(this, &ASFPilotPawn::LinearStiffnessBoostTimelineUpdate);
	BoostTimelineFinishedDelegate.BindDynamic(this, &ASFPilotPawn::BoostTimelineFinished);

	SpeedBoostDecay = 200.0f;
	LinearStiffnessBoostDecay = 50.0f;

	MaximumBoosterEnergy = 100.0f;
	MinimumEnergyToStartBoost = 25.0f;
	BoostEnergyDecayRate = 25.0f;
	BoostEnergyRegenRate = 5.0f;
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

	if (SpeedBoostCurve)
	{
		BoostTimeline->AddInterpFloat(SpeedBoostCurve, SpeedBoostTimelineUpdateDelegate, FName("Speed"));
		BoostTimeline->SetTimelineFinishedFunc(BoostTimelineFinishedDelegate);
	}

	if (LinearStiffnessBoostCurve)
		BoostTimeline->AddInterpFloat(LinearStiffnessBoostCurve, LinearStiffnessBoostTimelineUpdateDelegate, FName("Handling"));
	
	BoosterEnergy = MaximumBoosterEnergy;
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
	// Boost Speed / Handling Decay
	if (!bIsBoosting)
	{
		if (SpeedBoostSpeedDelta != 0.0)
		{
			float NextBoostTimelineSpeedDelta = SpeedBoostSpeedDelta - SpeedBoostDecay * DeltaTime;
			if (NextBoostTimelineSpeedDelta < 0.1)
				NextBoostTimelineSpeedDelta = 0.0f;

			SetSpeedBoostDelta(NextBoostTimelineSpeedDelta);
		}
		if (LinearStiffnessBoostDelta)
		{
			float NextBoostTimelineHandlingDelta = LinearStiffnessBoostDelta - LinearStiffnessBoostDecay * DeltaTime;
			if (NextBoostTimelineHandlingDelta < 0.1)
				NextBoostTimelineHandlingDelta = 0.0f;

			SetLinearStiffnessBoostDelta(NextBoostTimelineHandlingDelta);
		}
	}

	const float BoosterEnergyDelta = (bIsBoosting ? -BoostEnergyDecayRate : BoostEnergyRegenRate) * DeltaTime;
	BoosterEnergy = FMath::Clamp(BoosterEnergyDelta + BoosterEnergy, 0.0f, MaximumBoosterEnergy);
	if (bIsBoosting && BoosterEnergy == 0.0f)
	{
		TrySetIsBoosting(false);
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
	const EHandState HandState = Hand->GetHandState();
	if (HandState == EHandState::Driving)
		TrySetIsBoosting(bIsPressed);
	else if (HandState == EHandState::Ready)
		StartPilotingShip(Hand, Hand->GetOverlappingShip());
}

void ASFPilotPawn::TrySetIsBoosting(bool bNewIsBoosting)
{
	bool bSuccess = true;
	if (bNewIsBoosting)
	{
		bSuccess = BoosterEnergy > MinimumEnergyToStartBoost;
		if (bSuccess)
			BoostTimeline->PlayFromStart();

		OnBoostStart(bSuccess);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("LastBoostTimelineSpeedDelta STOP"))
		BoostTimeline->Stop();
		OnBoostStop();
	}

	if (bSuccess)
		bIsBoosting = bNewIsBoosting;
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
void ASFPilotPawn::SetSpeedBoostDelta(float NewBoostSpeed)
{
	SplineMovement->Speed = SplineMovement->Speed - SpeedBoostSpeedDelta + NewBoostSpeed;
	SpeedBoostSpeedDelta = NewBoostSpeed;
	UE_LOG(LogTemp, Warning, TEXT("LastBoostTimelineSpeedDelta 500 +%f = %f"), NewBoostSpeed, SplineMovement->Speed)
}

void ASFPilotPawn::SetLinearStiffnessBoostDelta(float NewBoostHandling)
{
	if (!Ship)
		return;

	const float NewLinearStiffness = Ship->FlightMovement->LinearStiffness - LinearStiffnessBoostDelta + NewBoostHandling;
	Ship->FlightMovement->SetLinearStiffness(NewLinearStiffness);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("NewLinearStiffness = %f"), NewLinearStiffness));
	LinearStiffnessBoostDelta = NewBoostHandling;
}

void ASFPilotPawn::SpeedBoostTimelineUpdate(float Value)
{
	SetSpeedBoostDelta(Value);
}

void ASFPilotPawn::LinearStiffnessBoostTimelineUpdate(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SetTimelineHandlingUpdate %f"), Value)
	SetLinearStiffnessBoostDelta(Value);
}

void ASFPilotPawn::BoostTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("BoostTimelineFinished"))
	if (BoostTimeline->GetPlaybackPosition() == 0.0f)
	{

	}
}
// Booster business end
