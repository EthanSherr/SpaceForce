// Fill out your copyright notice in the Description page of Project Settings.


#include "SFPilotPawn.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "../Environment/SFFlightPath.h"
#include "../Components/SFSplineMovementComponent.h"
#include "../Components/SFSpringFlightMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SFHandController.h"
#include "SpaceForce.h"
#include "SFShipPawn.h"
#include "SteamVRChaperoneComponent.h"
#include "DrawDebugHelpers.h"
#include "../UI/SFRadialMenuComponent.h"
#include "../Weapons/SFTurretActor.h"
#include "../UI/SFRadialMenuOption.h"
#include "../UI/SFRadialMenuComponent.h"
#include "Helpers/LoggingHelper.h"

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

void ASFPilotPawn::PostInitializeComponents() 
{
	Super::PostInitializeComponents();

	LeftHand->RadialMenuComponent->OnMenuItemSelected.AddDynamic(this, &ASFPilotPawn::MenuItemSelected);
	RightHand->RadialMenuComponent->OnMenuItemSelected.AddDynamic(this, &ASFPilotPawn::MenuItemSelected);
}

void ASFPilotPawn::MenuItemSelected(USFRadialMenuComponent* Menu, FSFRadialMenuOption Option, int Index)
{
	if (Menu->HandController->GetHandState() == EHandState::Aiming)
	{
		ActivateTurret(Index);
	}
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
	//hide the menus initially
	OnLeftTouchUp();
	OnRightTouchUp();
}

void ASFPilotPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateNextFlightPath();
	UpdateHandsRoot();
	UpdateThumbpadAxis();
	//if (Ship && Ship->GetActiveTurret())
	//{
	//	Ship->GetActiveTurret()->AimAt(GetHandInState(EHandState::Aiming)->GetComponentLocation());
	//}
}

void ASFPilotPawn::UpdateNextFlightPath()
{
	if (!Ship)
		return;
	auto NextFlightPath = GetHandInState(EHandState::Driving)->GetNearestFlightPath(SplineMovement->GetFlightPath());
	SplineMovement->NextFlightPath = NextFlightPath;
}

void ASFPilotPawn::UpdateHandsRoot()
{
	auto FlightPath = SplineMovement->GetFlightPath();
	if (!FlightPath) 
		return;
	const FVector Tangent = FlightPath->Spline->GetDirectionAtDistanceAlongSpline(SplineMovement->GetDistance(), ESplineCoordinateSpace::World);
	HandsRoot->SetRelativeLocation(HandExtension * Tangent);
}

void ASFPilotPawn::UpdateThumbpadAxis()
{
	if (!Ship)
		return;
	const FVector2D LeftInput = FVector2D(GetInputAxisValue("MCLeft_X"), GetInputAxisValue("MCLeft_Y"));
	const FVector2D RightInput = FVector2D(GetInputAxisValue("MCRight_X"), GetInputAxisValue("MCRight_Y"));
	LeftHand->RadialMenuComponent->SetAxisInput(LeftInput);
	RightHand->RadialMenuComponent->SetAxisInput(RightInput);
}

/** Start inputs */

void ASFPilotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &ASFPilotPawn::OnTriggerDownLeft);
	PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &ASFPilotPawn::OnTriggerDownRight);
	PlayerInputComponent->BindAction("LeftTrigger", IE_Released, this, &ASFPilotPawn::OnTriggerUpLeft);
	PlayerInputComponent->BindAction("RightTrigger", IE_Released, this, &ASFPilotPawn::OnTriggerUpRight);

	PlayerInputComponent->BindAction("LeftGrip", IE_Pressed, this, &ASFPilotPawn::OnLeftGripDown);
	PlayerInputComponent->BindAction("LeftGrip", IE_Released, this, &ASFPilotPawn::OnLeftGripUp);
	PlayerInputComponent->BindAction("RightGrip", IE_Pressed, this, &ASFPilotPawn::OnRightGripDown);
	PlayerInputComponent->BindAction("RightGrip", IE_Released, this, &ASFPilotPawn::OnRightGripUp);

	PlayerInputComponent->BindAction("LeftTouch", IE_Pressed, this, &ASFPilotPawn::OnLeftTouchDown);
	PlayerInputComponent->BindAction("LeftTouch", IE_Released, this, &ASFPilotPawn::OnLeftTouchUp);
	PlayerInputComponent->BindAction("RightTouch", IE_Pressed, this, &ASFPilotPawn::OnRightTouchDown);
	PlayerInputComponent->BindAction("RightTouch", IE_Released, this, &ASFPilotPawn::OnRightTouchUp);

	PlayerInputComponent->BindAction("LeftTap", IE_Pressed, this, &ASFPilotPawn::OnLeftClickDown);
	PlayerInputComponent->BindAction("RightTap", IE_Pressed, this, &ASFPilotPawn::OnRightClickDown);

	PlayerInputComponent->BindAxis("MCLeft_X");
	PlayerInputComponent->BindAxis("MCLeft_Y");
	PlayerInputComponent->BindAxis("MCRight_X");
	PlayerInputComponent->BindAxis("MCRight_Y");
}

void ASFPilotPawn::OnTriggerDownLeft()  { OnTrigger(LeftHand, true); }
void ASFPilotPawn::OnTriggerDownRight() { OnTrigger(RightHand, true); }
void ASFPilotPawn::OnTriggerUpLeft()    { OnTrigger(LeftHand, false); }
void ASFPilotPawn::OnTriggerUpRight()   { OnTrigger(RightHand, false); }

void ASFPilotPawn::OnLeftGripDown()	 { OnGrip(LeftHand, true); }
void ASFPilotPawn::OnLeftGripUp()	 { OnGrip(LeftHand, false); }
void ASFPilotPawn::OnRightGripDown() { OnGrip(RightHand, true); }
void ASFPilotPawn::OnRightGripUp()	 { OnGrip(RightHand, false); }

void ASFPilotPawn::OnLeftTouchDown()  { OnThumbpadTouch(LeftHand, true); }
void ASFPilotPawn::OnLeftTouchUp()    { OnThumbpadTouch(LeftHand, false); }
void ASFPilotPawn::OnRightTouchDown() { OnThumbpadTouch(RightHand, true); }
void ASFPilotPawn::OnRightTouchUp()   { OnThumbpadTouch(RightHand, false); }

void ASFPilotPawn::OnLeftClickDown()  { OnThumbpadClick(LeftHand, true); }
void ASFPilotPawn::OnRightClickDown() { OnThumbpadClick(RightHand, true); }

void ASFPilotPawn::OnTrigger(USFHandController* Hand, bool bIsPressed) {
	if (Hand->RecievesInput())
	{
		bool bCapturesInput;
		Hand->OnTrigger(bCapturesInput, bIsPressed);
		if (bCapturesInput)
			return;
	}
	switch (Hand->GetHandState()) {
	case EHandState::Ready:
		StartPilotingShip(Hand, Hand->GetOverlappingShip());
		break;
	case EHandState::Aiming:
		Ship->TriggerAction(bIsPressed);
		break;
	case EHandState::Driving:
		break;
	}
}

void ASFPilotPawn::OnGrip(USFHandController* Hand, bool bIsPressed)
{
	const EHandState HandState = Hand->GetHandState();
	if (HandState == EHandState::Driving)
	{
		Ship->TrySetIsBoosting(bIsPressed);
	}
	else if (HandState == EHandState::Ready)
	{
		StartPilotingShip(Hand, Hand->GetOverlappingShip());
	}
}

void ASFPilotPawn::OnThumbpadTouch(USFHandController* Hand, bool bIsPressed)
{
	if (bIsPressed && !Ship)
		return;
	Hand->RadialMenuComponent->SetVisibility(bIsPressed, true);
	Hand->RadialMenuComponent->LookAt = bIsPressed ? Camera : NULL;
}

void ASFPilotPawn::OnThumbpadClick(USFHandController* Hand, bool bIsPressed)
{
	if (!Ship)
		return;
	Hand->RadialMenuComponent->SelectFocusedIndex();
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
/** End inputs */

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
	if (!NewShip) 
		return;
	
	NewDrivingHand->SetHandState(EHandState::Driving);
	NewDrivingHand->RadialMenuComponent->SetData(DefensiveMenuOptions);
	NewDrivingHand->RadialMenuComponent->SetSelectedIndex(0);
	NewShip->FlightMovement->SetTargetComponent(NewDrivingHand);

	auto NewAimingHand = GetOtherHand(NewDrivingHand);
	NewAimingHand->SetHandState(EHandState::Aiming);

	NewShip->SetOwner(this);
	NewShip->AimTargetComponent = NewAimingHand;
	Ship = NewShip;
	ReceiveStartPilotingShip();
	NewShip->OnPossessed();

	TArray<FSFRadialMenuOption> OffensiveMenu;
	for (ASFTurretActor* TurretActor: Ship->Turrets)
	{
		const FSFRadialMenuOption MenuOption(TurretActor->DisplayName, TurretActor->MaterialIcon);
		OffensiveMenu.Add(MenuOption);
	}
	if (OffensiveMenu.Num() > 0)
	{
		ActivateTurret(0);
	}
	NewAimingHand->RadialMenuComponent->SetData(OffensiveMenu);
	NewAimingHand->RadialMenuComponent->SetSelectedIndex(0);
}

void ASFPilotPawn::ActivateTurret(int Index)
{
	USFHandController* Hand = GetHandInState(EHandState::Aiming);
	if (!Ship || !Hand) 
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateTurret failed, null found Hand: %s, Ship: %s"), *ULoggingHelper::GetNameOrNull(Hand), *ULoggingHelper::GetNameOrNull(Ship))
		return;
	}
	ASFTurretActor* Turret = Ship->GetActiveTurret();
	if (Turret)
	{
		UE_LOG(LogTemp, Warning, TEXT("Deactivating previous turret %s"), *Turret->GetName())
		Turret->AimAtComponent(NULL);
		Turret->SetActivated(false);
	}

	Turret = Ship->ActivateTurret(Index);
	if (!Turret) 
	{
		UE_LOG(LogTemp, Error, TEXT("ActivateTurret failed, NULL for index %d"), Index)
		return;
	}
	Turret->AimAtComponent(Hand);
	Turret->SetActivated(true);
}