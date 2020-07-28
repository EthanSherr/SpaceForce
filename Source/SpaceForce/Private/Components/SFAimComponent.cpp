// Fill out your copyright notice in the Description page of Project Settings.


#include "SFAimComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "SFTurretDelegate.h"
#include "MyBlueprintFunctionLibrary.h"
#include "GameFramework/MovementComponent.h"

// Sets default values for this component's properties
USFAimComponent::USFAimComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AimSpeed = 30.0f;
}

bool USFAimComponent::WasInitialized(bool logError)
{
	bool wasInit = BarrelSocket != NULL && MuzzleSocket != NULL && SkeletalMesh != NULL;
	if (!wasInit && logError) {
		UE_LOG(LogTemp, Error, TEXT("Unable to initialize aim component, barrel or muzzle sockets were invalid.  Check your socket names."))
	}
	return wasInit;
}

void USFAimComponent::Initialize(USkeletalMeshComponent* SKM, float projectileSpeed, FName Barrel, FName Muzzle)
{
	SkeletalMesh = SKM;
	BarrelSocket = SKM->GetSocketByName(Barrel);
	MuzzleSocket = SKM->GetSocketByName(Muzzle);
	ProjectileSpeed = projectileSpeed;
	if (!WasInitialized(true)) {
		return;
	}
	InitialBarrelTransformCS = BarrelSocket->GetSocketTransform(SKM) * SkeletalMesh->GetComponentTransform().Inverse();
	UE_LOG(LogTemp, Warning, TEXT("%s InitialBarrelTransform %s"), *GetOwner()->GetName(), *InitialBarrelTransformCS.ToString())
	BarrelLength = FVector::Distance(BarrelSocket->GetSocketLocation(SKM), MuzzleSocket->GetSocketLocation(SKM));
}

void USFAimComponent::AimAt(FVector target) {
	if (!WasInitialized(true)) {
		return;
	}
	bWasTargetSet = true;
	Target = target;
}

void USFAimComponent::AimAtActor(AActor* actor, bool withLead) {
	if (!WasInitialized(true)) {
		return;
	}
	bWasTargetSet = actor != NULL;
	TrackedActor = actor;
	bLeadTrackedActor = withLead;
}

void USFAimComponent::Debug() 
{
	FTransform t = InitialBarrelTransformCS * SkeletalMesh->GetComponentTransform();
	FVector forward = t.Rotator().RotateVector(FVector::ForwardVector);

	DrawDebugLine(GetWorld(), t.GetLocation(), forward * 100 + t.GetLocation(), FColor::Green, false, 0, 1, 1);
}

FAimCallibration USFAimComponent::GetAimCallibration() {
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

bool USFAimComponent::UpdateTargetFromTrackedActor()
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

bool USFAimComponent::IsAimingAtTarget(float tolerance) {

	if (!bWasTargetSet) {
		return false;
	}

	FTransform socketTransformWorld = BarrelSocket->GetSocketTransform(SkeletalMesh);
	FRotator barrelRotation = socketTransformWorld.Rotator();
	FRotator targetRotation = FRotationMatrix::MakeFromX(Target - socketTransformWorld.GetLocation()).Rotator();
	FRotator deltaRotator = barrelRotation - targetRotation;

	return deltaRotator.Pitch <= tolerance && deltaRotator.Yaw <= tolerance;
}

bool USFAimComponent::Fire() {
	if (!GetOwner()->Implements<USFTurretDelegate>()) {
		return false;
	}
	return ISFTurretDelegate::Execute_Fire(GetOwner(), this);
}

FTransform USFAimComponent::GetMuzzleTransform() {
	return MuzzleSocket->GetSocketTransform(SkeletalMesh);
}

FTransform USFAimComponent::GetBarrelTransform() {
	return BarrelSocket->GetSocketTransform(SkeletalMesh);
}