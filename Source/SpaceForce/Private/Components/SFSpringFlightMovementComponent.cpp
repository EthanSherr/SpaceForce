// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSpringFlightMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Helpers/LoggingHelper.h"

USFSpringFlightMovementComponent::USFSpringFlightMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;
	LinearStiffness = 25.0f;
	LinearCriticalDamping = 0.7f;
	LinearMaxSpeed = 40.0f;
	AngularStiffnessPrimary = 20.0f;
	AngularStiffnessSecondary = 70.0f;
	AngularDampingPrimary = 7.0f;
	AngularDampingSecondary = 7.0f;
	bMaintainMaxSpeed = false;
}

bool USFSpringFlightMovementComponent::IsValid(bool logError) {
	auto primitive = GetUpdatedPrimitiveComp();
	bool bIsValid = primitive != NULL && primitive->IsSimulatingPhysics();
	if (!bIsValid && logError) {
		UE_LOG(LogTemp, Error, TEXT("%s's root component (%s) does not simulate physics, can't apply spring forces."),
			*GetOwner()->GetName(),
			*GetOwner()->GetRootComponent()->GetName())
	}
	return bIsValid;
}

void USFSpringFlightMovementComponent::AddInputVector(FVector WorldVector, bool bForce) {
	if (!GetUpdatedPrimitiveComp())
	{
		UE_LOG(LogTemp, Error, TEXT("SpringFlightMovement failed for %s, rootComp is %s"), *GetOwner()->GetName(), *ULoggingHelper::GetNameOrNull(GetOwner()->GetRootComponent()))
		return;
	}
	const FVector MoveTarget = WorldVector + GetUpdatedPrimitiveComp()->GetComponentLocation();
	//DrawDebugLine(GetWorld(), GetUpdatedPrimitiveComp()->GetComponentLocation(), Target, FColor::Yellow, false, 0.f, 10, 0.5f);
	//DrawDebugPoint(GetWorld(), Target, 5.0f, FColor::Yellow, false, 0.f, 10);
	SetTarget(MoveTarget);
	Super::AddInputVector(WorldVector, bForce);
	ConsumeInputVector();
	//PendingInputVector grows until cleared with ConsumeInputVector().
}

void USFSpringFlightMovementComponent::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(true))
		return;
	
	RecomputeSpringConfig();

	if (InitialTarget) 
		SetTargetComponent(InitialTarget->GetRootComponent());
}

void USFSpringFlightMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bHasTarget || !IsValid()) {
		return;
	}

	UPrimitiveComponent* Prim = GetUpdatedPrimitiveComp();
	UpdateTarget(DeltaTime);

	FVector ShipForward = Prim->GetForwardVector();
	FVector ShipUp = Prim->GetUpVector();
	FVector ShipRight = Prim->GetRightVector();
	FVector ShipVelocity = Prim->GetComponentVelocity();
	FVector ShipAngularVelocity = Prim->GetPhysicsAngularVelocityInRadians();

	FVector TargetForward = ShipVelocity.GetSafeNormal();
	FVector TargetUp = FRotationMatrix::MakeFromX(TargetForward).Rotator().RotateVector(FVector::UpVector);
	if (bUseTargetOrientation && TargetComponent) {
		FVector targetUp = TargetComponent->GetUpVector();
		TargetUp = ShipUp * FVector::DotProduct(ShipUp, targetUp) + ShipRight * FVector::DotProduct(ShipRight, targetUp);
	}

	FVector Force = CalculateSpringDampingForces(Prim->GetComponentLocation(), Target, ShipVelocity, TargetVelocity, SpringConfig.Stiffness, SpringConfig.Damping, SpringConfig.MaxExtension);
	FVector PrimaryTorque = CalculateSpringDampingTorque(ShipForward, TargetForward, ShipAngularVelocity, AngularStiffnessPrimary, AngularDampingPrimary);
	FVector SecondaryTorque = CalculateSpringDampingTorque(ShipUp, TargetUp, ShipAngularVelocity, AngularStiffnessSecondary, AngularDampingSecondary);

	FVector Start = Prim->GetComponentLocation();

	if (bDebugRotation)
	{
		DrawDebugLine(GetWorld(), Start, Start + TargetUp * 100, FColor::Green, false, 0, 5, 2.0f);
		DrawDebugLine(GetWorld(), Start, Start + ShipUp * 50, FColor::Yellow, false, 0, 7, 2.0f);
	}

	if (bDebugTorque)
	{
		DrawDebugLine(GetWorld(), Start, Start + PrimaryTorque * 100, FColor::Blue, false, 0, 5, 1.0f);
		DrawDebugLine(GetWorld(), Start, Start + SecondaryTorque * 100, FColor::Purple, false, 0, 8, 1.0f);
	}

	if (bDebugTarget)
	{
		DrawDebugPoint(GetWorld(), Target, 10, FColor::Yellow, false, 0.0f, 5);
	}

	Prim->AddForce(Force);
	Prim->AddTorqueInRadians(PrimaryTorque + SecondaryTorque, FName(), true);
	LastTargetPosition = Target;
}


void USFSpringFlightMovementComponent::UpdateTarget(float DeltaTime)
{
	if (TargetComponent != NULL) {
		Target = TargetComponent->GetComponentLocation();
		TargetVelocity = TargetComponent->GetComponentVelocity();
	}

	if (bCalculateVelocityOfTarget) {
		TargetVelocity = (Target - LastTargetPosition) / DeltaTime;
		LastTargetPosition = Target;
	}
}

// Calculate Forces
FVector USFSpringFlightMovementComponent::CalculateForces(FVector P2, FVector P2Velocity) {
	auto Primitive = GetUpdatedPrimitiveComp();
	FVector DeltaL = P2 - Primitive->GetComponentLocation();
	FVector DeltaV = P2Velocity - Primitive->GetComponentVelocity();

	if (SpringConfig.MaxExtension > 0.0f && (bMaintainMaxSpeed || SpringConfig.MaxExtension < DeltaL.Size())) {
		DeltaL = DeltaL.GetSafeNormal() * SpringConfig.MaxExtension;
		if (bDebug) {
			DrawDebugLine(GetWorld(), Primitive->GetComponentLocation() + DeltaL, P2, FColor::Red, false, 0, 0, 1);
			DrawDebugLine(GetWorld(), Primitive->GetComponentLocation(), Primitive->GetComponentLocation() + DeltaL, FColor::Green, false, 0, 0, 1);
		}
	} else if (bDebug) {
		DrawDebugLine(GetWorld(), Primitive->GetComponentLocation(), P2, FColor::Blue, false, 0, 0, 1);
	}
	FVector Fs = SpringConfig.Stiffness * DeltaL;
	FVector Fd = SpringConfig.Damping * DeltaV;
	return Fs + Fd;
}

FVector USFSpringFlightMovementComponent::CalculateSpringDampingForces(FVector CurrentLocation, FVector TargetLocation, FVector CurrentVelocity, FVector InTargetVelocity, float Ks, float Kd, float MaxExtension) {
	FVector DLocation = TargetLocation - CurrentLocation;
	FVector DVelocity = InTargetVelocity - CurrentVelocity;
	if (MaxExtension > 0.0f && (bMaintainMaxSpeed || DLocation.Size() > MaxExtension)) {
		DLocation = DLocation.GetSafeNormal() * MaxExtension;
		if (bDebug) {
			DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + DLocation, FColor::Green, false, 0, 0, 1);
			DrawDebugLine(GetWorld(), CurrentLocation + DLocation, TargetLocation, FColor::Red, false, 0, 0, 1);
		}
	} else if (bDebug) {
		DrawDebugLine(GetWorld(), CurrentLocation, TargetLocation, FColor::Blue, false, 0, 0, 1);
	}
	FVector Fs = Ks * DLocation;
	FVector Fd = Kd * DVelocity;
	return Fs + Fd;
}

FVector USFSpringFlightMovementComponent::CalculateSpringDampingTorque(FVector CurrentOrientation, FVector TargetOrientation, FVector AngularVelocity, float Ks, float Kd) {
	FQuat rot = FQuat::FindBetween(CurrentOrientation, TargetOrientation);
	FVector Fs = Ks * FVector(rot.X, rot.Y, rot.Z);
	FVector Fd = -Kd * AngularVelocity;
	return Fs + Fd;
}

// Begin Targeting Methods
void USFSpringFlightMovementComponent::SetTarget(FVector value) {
	Target = value;
	if (!bHasTarget) {
		LastTargetPosition = value;
		bHasTarget = true;
	}
}

void USFSpringFlightMovementComponent::SetTargetComponent(USceneComponent* NewTargetComponent) {
	TargetComponent = NewTargetComponent;
	if (TargetComponent != NULL) {
		LastTargetPosition = TargetComponent->GetComponentLocation();
		bHasTarget = true;
	} else {
		bHasTarget = false;
	}
}

void USFSpringFlightMovementComponent::SetSpeed(float NewLinearMaxSpeed) 
{
	LinearMaxSpeed = NewLinearMaxSpeed;
	RecomputeSpringConfig();
}

void USFSpringFlightMovementComponent::SetLinearStiffness(float NewLinearStiffness)
{
	LinearStiffness = NewLinearStiffness;
	RecomputeSpringConfig();
}

void USFSpringFlightMovementComponent::RecomputeSpringConfig()
{
	if (!IsValid(true))
		return;
	SpringConfig = FSpringConfig::FromCriticalDampingAndMaxSpeed(LinearStiffness, LinearCriticalDamping, LinearMaxSpeed, GetUpdatedPrimitiveComp()->GetMass());
}

FVector USFSpringFlightMovementComponent::GetTarget() {
	return Target;
}

USceneComponent* USFSpringFlightMovementComponent::GetTargetComponent() {
	return TargetComponent;
}

void USFSpringFlightMovementComponent::ClearTarget() {
	LastTargetPosition = TargetVelocity = Target = FVector::ZeroVector;
	TargetComponent = NULL;
	bHasTarget = false;
}
// End Targeting Methods

UPrimitiveComponent* USFSpringFlightMovementComponent::GetUpdatedPrimitiveComp() {
	return Cast<UPrimitiveComponent>(UpdatedComponent);
}