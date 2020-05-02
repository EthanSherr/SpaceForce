// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSpringFlightMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

USFSpringFlightMovementComponent::USFSpringFlightMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;
	LinearStiffness = 25.0f;
	LinearCriticalDamping = 0.7f;
	LinearMaxSpeed = 40.0f;
	AngularStiffness = 20.0f;
	AngularDamping = 7.0f;
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
	SetTarget(WorldVector + GetUpdatedPrimitiveComp()->GetComponentLocation());
	//Super::AddInputVector(WorldVector, bForce);
	//PendingInputVector grows until cleared with ConsumeInputVector().
}

void USFSpringFlightMovementComponent::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(true)) {
		return;
	}
	SpringConfig = FSpringConfig::FromCriticalDampingAndMaxSpeed(
		LinearStiffness, 
		LinearCriticalDamping, 
		LinearMaxSpeed, 
		GetUpdatedPrimitiveComp()->GetMass());
	if (InitialTarget) {
		SetTargetActor(InitialTarget);
	}
}

void USFSpringFlightMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bHasTarget || !IsValid()) {
		return;
	}

	auto prim = GetUpdatedPrimitiveComp();
	FVector forward = prim->GetComponentVelocity();
	UpdateTarget(DeltaTime);
	prim->AddForce(CalculateForces(Target, TargetVelocity));
	prim->AddTorqueInRadians(CalculateTorque(forward), FName(), true);
}

void USFSpringFlightMovementComponent::UpdateTarget(float DeltaTime)
{
	if (TargetActor != NULL) {
		Target = TargetActor->GetActorLocation();
		TargetVelocity = TargetActor->GetVelocity();
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

FVector USFSpringFlightMovementComponent::CalculateTorque(FVector forward) {
	auto prim = GetUpdatedPrimitiveComp();

	FVector L = prim->GetComponentLocation();
	FVector vForward = forward.GetSafeNormal();
	FVector vUp = FRotator(90, 0, 0).RotateVector(vForward);

	FQuat rot = FQuat::FindBetween(prim->GetForwardVector(), vForward); // +FQuat::FindBetween(prim->GetUpVector(), vUp);
	FVector Fr = AngularStiffness * FVector(rot.X, rot.Y, rot.Z);

	if (bDebugRotation) {
		DrawDebugLine(GetWorld(), L, L + vForward * 100, FColor::Red, false, 0, 0, 1);
		DrawDebugLine(GetWorld(), L, L + GetUpdatedPrimitiveComp()->GetForwardVector() * 50, FColor::Purple, false, 0, 1, 1);
		DrawDebugLine(GetWorld(), L, L + Fr, FColor::Yellow, false, 0, 2, 1);
	}
	FVector Fd = -AngularDamping * prim->GetPhysicsAngularVelocityInRadians();
	return Fr + Fd;
}

// Begin Targeting Methods
void USFSpringFlightMovementComponent::SetTarget(FVector value) {
	Target = value;
	LastTargetPosition = value;
	bHasTarget = true;
}

void USFSpringFlightMovementComponent::SetTargetActor(AActor* value) {
	TargetActor = value;
	if (value != NULL) {
		LastTargetPosition = TargetActor->GetActorLocation();
		bHasTarget = true;
	} else {
		bHasTarget = false;
	}
}

void USFSpringFlightMovementComponent::SetSpeed(float Speed) {
	if (!IsValid(true)) {
		return;
	}
	LinearMaxSpeed = Speed;
	SpringConfig = FSpringConfig::FromCriticalDampingAndMaxSpeed(LinearStiffness, LinearCriticalDamping, Speed, GetUpdatedPrimitiveComp()->GetMass());
}

FVector USFSpringFlightMovementComponent::GetTarget() {
	return Target;
}

AActor* USFSpringFlightMovementComponent::GetTargetActor() {
	return TargetActor;
}

void USFSpringFlightMovementComponent::ClearTarget() {
	LastTargetPosition = TargetVelocity = Target = FVector::ZeroVector;
	TargetActor = NULL;
	bHasTarget = false;
}
// End Targeting Methods

UPrimitiveComponent* USFSpringFlightMovementComponent::GetUpdatedPrimitiveComp() {
	return Cast<UPrimitiveComponent>(UpdatedComponent);
}