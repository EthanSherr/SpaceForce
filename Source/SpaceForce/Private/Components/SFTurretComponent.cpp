// Fill out your copyright notice in the Description page of Project Settings.


#include "SFTurretComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "SFTurretDelegate.h"
#include "MyBlueprintFunctionLibrary.h"
#include "GameFramework/MovementComponent.h"

// Sets default values for this component's properties
USFTurretComponent::USFTurretComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "SkeletalMesh");
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
	UE_LOG(LogTemp, Warning, TEXT("%s InitialBarrelTransform %s"), *GetOwner()->GetName(), *InitialBarrelTransformCS.ToString())
	BarrelLength = FVector::Distance(BarrelSocket->GetSocketLocation(SkeletalMesh), MuzzleSocket->GetSocketLocation(SkeletalMesh));
}

void USFTurretComponent::AimAt(FVector target) {
	if (!WasInitialized(true)) {
		return;
	}
	bWasTargetSet = true;
	Target = target;
}

void USFTurretComponent::AimAtActor(AActor* actor, bool withLead) {
	if (!WasInitialized(true)) {
		return;
	}
	bWasTargetSet = actor != NULL;
	TrackedActor = actor;
	bLeadTrackedActor = withLead;
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

bool USFTurretComponent::UpdateTargetFromTrackedActor()
{
	if (TrackedActor == NULL) {
		return false;
	}
	if (!bLeadTrackedActor) {
		Target = TrackedActor->GetActorLocation();
		return true;
	}
	FProjectilePredictionResult result = UMyBlueprintFunctionLibrary::ComputeProjectilePrediction(
		TrackedActor->GetActorLocation(),
		TrackedActor->GetVelocity(),
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

bool USFTurretComponent::Fire() {
	if (!GetOwner()->Implements<USFTurretDelegate>()) {
		return false;
	}
	return false;
//	return ISFTurretDelegate::Execute_Fire(GetOwner(), this);
}

FTransform USFTurretComponent::GetMuzzleTransform() {
	return MuzzleSocket->GetSocketTransform(SkeletalMesh);
}

FTransform USFTurretComponent::GetBarrelTransform() {
	return BarrelSocket->GetSocketTransform(SkeletalMesh);
}
