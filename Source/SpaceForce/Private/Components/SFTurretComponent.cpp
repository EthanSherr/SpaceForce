// Fill out your copyright notice in the Description page of Project Settings.


#include "SFTurretComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Helpers/SFMath.h"
#include "GameFramework/MovementComponent.h"
#include "Helpers/LoggingHelper.h"

// Sets default values for this component's properties
USFTurretComponent::USFTurretComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "TurretSKM");
	SkeletalMesh->SetupAttachment(this);

	PrimaryComponentTick.bCanEverTick = false;
	AimSpeed = 30.0f;

	MuzzleName = FName("Muzzle");
	BarrelName = FName("Barrel");
}

void USFTurretComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

bool USFTurretComponent::WasInitialized(bool logError)
{
	bool wasInit = BarrelSocket != NULL && MuzzleSocket != NULL && SkeletalMesh != NULL;
	if (!wasInit && logError) {
		UE_LOG(LogTemp, Error, TEXT("Unable to initialize aim component, barrel or muzzle sockets were invalid.  Check your socket names."))
	}
	return wasInit;
}

void USFTurretComponent::Initialize()
{
	BarrelSocket = SkeletalMesh->GetSocketByName(BarrelName);
	MuzzleSocket = SkeletalMesh->GetSocketByName(MuzzleName);
	if (!WasInitialized(true)) {
		return;
	}
	InitialBarrelTransformCS = BarrelSocket->GetSocketTransform(SkeletalMesh) * SkeletalMesh->GetComponentTransform().Inverse();
	//UE_LOG(LogTemp, Warning, TEXT("%s InitialBarrelTransform %s"), *GetOwner()->GetName(), *InitialBarrelTransformCS.ToString())
	BarrelLength = FVector::Distance(BarrelSocket->GetSocketLocation(SkeletalMesh), MuzzleSocket->GetSocketLocation(SkeletalMesh));
}

void USFTurretComponent::AimAt(FVector target) {
	if (!WasInitialized(false)) {
		return;
	}
	bWasTargetSet = true;
	Target = target;
}

void USFTurretComponent::AimAtComponent(USceneComponent* Component) {
	if (!WasInitialized(false)) {
		return;
	}
	bWasTargetSet = Component != NULL;
	TrackedComponent = Component;
}

void USFTurretComponent::Debug()
{
	FTransform t = InitialBarrelTransformCS * SkeletalMesh->GetComponentTransform();
	FVector forward = t.Rotator().RotateVector(FVector::ForwardVector);

	DrawDebugLine(GetWorld(), t.GetLocation(), forward * 100 + t.GetLocation(), FColor::Green, false, 0, 1, 1);
}

FAimCallibration USFTurretComponent::GetAimCallibration() {
	if (!bWasTargetSet) {
		return FAimCallibration();
	}

	UpdateTargetFromTrackedActor();

	FTransform initialBarrelWS = InitialBarrelTransformCS * SkeletalMesh->GetComponentTransform();
	FVector initialBarrelForwardWS = initialBarrelWS.Rotator().RotateVector(FVector::ForwardVector);
	FTransform socketTransformWS = BarrelSocket->GetSocketTransform(SkeletalMesh);

	FRotator from = socketTransformWS.Rotator() - initialBarrelWS.Rotator();
	FTransform t = socketTransformWS * initialBarrelWS.Inverse();

	FVector relativeTarget = initialBarrelWS.Rotator().UnrotateVector(Target - socketTransformWS.GetTranslation());
	FRotator to = FRotationMatrix::MakeFromX(relativeTarget).Rotator();

	return FAimCallibration(from, to);
}

void USFTurretComponent::DebugTransform(FTransform transform, float time)
{
	if (!bDebug)
		return;
	const float Magnitude = 25.0f;
	auto Origin = transform.GetLocation();
	auto Forward = transform.GetRotation().RotateVector(FVector::ForwardVector);
	auto Right = transform.GetRotation().RotateVector(FVector::RightVector);
	auto Up = transform.GetRotation().RotateVector(FVector::UpVector);

	DrawDebugLine(GetWorld(), Origin, Origin + Forward * Magnitude, FColor::Red, false, time, 3, 1);
	DrawDebugLine(GetWorld(), Origin, Origin + Right * Magnitude, FColor::Green, false, time, 3, 1);
	DrawDebugLine(GetWorld(), Origin, Origin + Up * Magnitude, FColor::Blue, false, time, 3, 1);
}

FAimCallibration USFTurretComponent::GetAimCallibrationOld() {
	if (!bWasTargetSet) {
		return FAimCallibration();
	}

	UpdateTargetFromTrackedActor();

	FTransform InitialBarrelWS = InitialBarrelTransformCS * SkeletalMesh->GetComponentTransform();
	DebugTransform(InitialBarrelWS);

	FVector InitialBarrelForward = InitialBarrelWS.GetRotation().RotateVector(FVector::ForwardVector);
	FVector DirectionOfTarget = (Target - InitialBarrelWS.GetLocation()).GetSafeNormal();
	FRotator DR = DirectionOfTarget.Rotation() - InitialBarrelForward.Rotation();

	//const FVector TransformedTarget = InitialBarrelWS.Inverse().TransformPosition(Target);
	//const FRotator to = FRotationMatrix::MakeFromX(TransformedTarget).Rotator();
	DrawDebugLine(GetWorld(), InitialBarrelWS.GetLocation(), InitialBarrelWS.GetLocation() + DirectionOfTarget * 30, FColor::Yellow, false, 0, 3, 1);


	//FVector initialBarrelForwardWS = InitialBarrelWS.Rotator().RotateVector(FVector::ForwardVector);

	//FTransform socketTransformWS = BarrelSocket->GetSocketTransform(SkeletalMesh);

	//DebugTransform(socketTransformWS);

	//FRotator from = socketTransformWS.Rotator() - InitialBarrelWS.Rotator();
	//FTransform t = socketTransformWS * InitialBarrelWS.Inverse();

	//FVector relativeTarget = InitialBarrelWS.Rotator().UnrotateVector(Target - socketTransformWS.GetTranslation());
	//FRotator to = FRotationMatrix::MakeFromX(relativeTarget).Rotator();

	return FAimCallibration(FRotator(), DR);//FAimCallibration(FRotator(),to);
}

bool USFTurretComponent::UpdateTargetFromTrackedActor()
{
	if (TrackedComponent == NULL) {
		return false;
	}
	if (!bLeadTarget) {
		Target = TrackedComponent->GetComponentLocation();
		return true;
	}
	//TrackedComponent->GetVelocity()// TODO?
	FVector Velocity = TrackedComponent->GetComponentVelocity();
	FProjectilePredictionResult result = USFMath::ComputeProjectilePrediction(
		TrackedComponent->GetComponentLocation(),
		Velocity,
		GetBarrelTransform().GetLocation(),
		ProjectileSpeed,
		BarrelLength);
	if (result.bSuccess) {
		Target = result.predictedImpact;
	}
	return result.bSuccess;
}

bool USFTurretComponent::IsAimingAtTarget(float tolerance) {

	if (!bWasTargetSet) {
		return false;
	}

	FTransform socketTransformWorld = BarrelSocket->GetSocketTransform(SkeletalMesh);
	FRotator barrelRotation = socketTransformWorld.Rotator();
	FRotator targetRotation = FRotationMatrix::MakeFromX(Target - socketTransformWorld.GetLocation()).Rotator();
	FRotator deltaRotator = barrelRotation - targetRotation;

	return deltaRotator.Pitch <= tolerance && deltaRotator.Yaw <= tolerance;
}

FTransform USFTurretComponent::GetMuzzleTransform() {
	if (!MuzzleSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s muzzle socket is null"), *GetName())
		return FTransform::Identity;
	}
	FTransform Transform = MuzzleSocket->GetSocketTransform(SkeletalMesh);
	Transform.RemoveScaling();
	return Transform;
}

FTransform USFTurretComponent::GetBarrelTransform() {
	return BarrelSocket->GetSocketTransform(SkeletalMesh);
}
