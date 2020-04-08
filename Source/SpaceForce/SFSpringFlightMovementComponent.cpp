// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSpringFlightMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

USFSpringFlightMovementComponent::USFSpringFlightMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	SpringConfig = FSpringConfig(25.0f, 0.7f, 50.0f);
}

bool USFSpringFlightMovementComponent::IsValid(bool logError) {
	auto primitive = GetUpdatedPrimitiveComp();
	bool bIsValid = primitive != NULL && primitive->IsSimulatingPhysics();
	if (!bIsValid && logError) {
		UE_LOG(LogTemp, Warning, TEXT("%s's root component (%s) does not simulate physics, can't apply spring forces."),
			*GetOwner()->GetName(),
			*GetOwner()->GetRootComponent()->GetName())
	}
	return bIsValid;
}

void USFSpringFlightMovementComponent::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(true)) {
		return;
	}
	DampingCoefficient = ComputeDampingCoefficient(SpringConfig, (GetUpdatedPrimitiveComp()->GetMass()));
}

void USFSpringFlightMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!bHasTarget || !IsValid()) {
		return;
	}
	UpdateTarget();

	FVector forces = CalculateForces();
	if (bDebugRotation && GetUpdatedPrimitiveComp()->GetComponentVelocity().Size() > 30) {

		FVector L = GetUpdatedPrimitiveComp()->GetComponentLocation();
		FVector vForward = GetUpdatedPrimitiveComp()->GetComponentVelocity().GetSafeNormal();
		FVector vUp = FRotator(90, 0, 0).RotateVector(vForward);

		FVector vRight = FVector::CrossProduct(vUp, vForward);

		auto velocityRotation = FRotationMatrix::MakeFromXY(vForward, vRight);
		GetUpdatedPrimitiveComp()->SetWorldRotation(velocityRotation.Rotator());

		DrawDebugLine(GetWorld(), L, L + vForward * 100, FColor::Red, false, 0, 0, 1);
		DrawDebugLine(GetWorld(), L, L + vUp * 100, FColor::Blue, false, 0, 0, 1);
		DrawDebugLine(GetWorld(), L, L + vRight * 100, FColor::Green, false, 0, 0, 1);

		DrawDebugLine(GetWorld(), L, L + GetUpdatedPrimitiveComp()->GetForwardVector() * 50, FColor::Purple, false, 0, 1, 1);
	}
	GetUpdatedPrimitiveComp()->AddForce(forces);
}

void USFSpringFlightMovementComponent::UpdateTarget()
{
	if (TargetActor != NULL) {
		Target = TargetActor->GetActorLocation();
		TargetVelocity = TargetActor->GetVelocity();
//		UE_LOG(LogTemp, Warning, TEXT("Target %s TargetVelocity %s"), *Target.ToString(), *TargetVelocity.ToString())
	}
}

// Calculate Forces
FVector USFSpringFlightMovementComponent::CalculateForces() {
	auto Primitive = GetUpdatedPrimitiveComp();
	FVector DeltaL = Target - Primitive->GetComponentLocation();
	FVector DeltaV = TargetVelocity - Primitive->GetComponentVelocity();
	if (SpringConfig.MaxExtension > 0.0f && SpringConfig.MaxExtension < DeltaL.Size()) {
		DeltaL = DeltaL.GetSafeNormal() * SpringConfig.MaxExtension;
		if (bDebug) {
			DrawDebugLine(GetWorld(), Primitive->GetComponentLocation() + DeltaL, Target, FColor::Red, false, 0, 0, 1);
			DrawDebugLine(GetWorld(), Primitive->GetComponentLocation(), Primitive->GetComponentLocation() + DeltaL, FColor::Green, false, 0, 0, 1);
		}
	} else if (bDebug) {
		DrawDebugLine(GetWorld(), Primitive->GetComponentLocation(), Target, FColor::Green, false, 0, 0, 1);
	}
	FVector Fs = SpringConfig.Stiffness * DeltaL;
	FVector Fd = DampingCoefficient * DeltaV;
	return Fs + Fd;
}

float USFSpringFlightMovementComponent::ComputeDampingCoefficient(FSpringConfig config, float mass) {
	return config.CriticalDampingCoefficient * 2 * FMath::Sqrt(config.Stiffness * mass);
}

// Begin Targeting Methods
void USFSpringFlightMovementComponent::SetTarget(FVector value) {
	Target = value;
	bHasTarget = true;
}

void USFSpringFlightMovementComponent::SetTargetActor(AActor* value) {
	TargetActor = value;
	bHasTarget = true;
}

FVector USFSpringFlightMovementComponent::GetTarget() {
	return Target;
}

AActor* USFSpringFlightMovementComponent::GetTargetActor() {
	return TargetActor;
}

void USFSpringFlightMovementComponent::ClearTarget() {
	TargetVelocity = Target = FVector::ZeroVector;
	TargetActor = NULL;
	bHasTarget = false;
}
// End Targeting Methods

UPrimitiveComponent* USFSpringFlightMovementComponent::GetUpdatedPrimitiveComp() {
	return Cast<UPrimitiveComponent>(UpdatedComponent);
}