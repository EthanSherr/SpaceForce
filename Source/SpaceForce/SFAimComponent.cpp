// Fill out your copyright notice in the Description page of Project Settings.


#include "SFAimComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
USFAimComponent::USFAimComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool USFAimComponent::WasInitialized(bool logError)
{
	bool wasInit = BarrelSocket != NULL && MuzzleSocket != NULL && SkeletalMesh != NULL;
	if (!wasInit && logError) {
		UE_LOG(LogTemp, Error, TEXT("Unable to initialize aim component, barrel or muzzle sockets were invalid.  Check your socket names."))
	}
	return wasInit;
}

void USFAimComponent::Initialize(USkeletalMeshComponent* SKM, FName Barrel, FName Muzzle)
{
	SkeletalMesh = SKM;
	BarrelSocket = SKM->GetSocketByName(Barrel);
	MuzzleSocket = SKM->GetSocketByName(Muzzle);
	if (!WasInitialized(true)) {
		return;
	}
	BarrelLength = FVector::Distance(BarrelSocket->GetSocketLocation(SKM), MuzzleSocket->GetSocketLocation(SKM));

	FTransform socketTransform = BarrelSocket->GetSocketTransform(SkeletalMesh);
	FRotator socketRotationInComponentSpace = socketTransform.Rotator() - SkeletalMesh->GetComponentRotation();

	UE_LOG(LogTemp, Warning, TEXT("socketRotationInComponentSpace %s"), *socketRotationInComponentSpace.ToString())

}

void USFAimComponent::AimAt(FVector target)
{
	if (!WasInitialized()) {
		return;
	}
	bWasTargetSet = true;
	Target = target;
}

FAimCallibration USFAimComponent::GetAimCallibration() {
	if (!bWasTargetSet) {
		return FAimCallibration();
	}
	FTransform socketTransformWorld = BarrelSocket->GetSocketTransform(SkeletalMesh);

	FVector relativeTarget = GetOwner()->GetActorRotation().UnrotateVector(Target - socketTransformWorld.GetTranslation());
	FRotator to = FRotationMatrix::MakeFromX(relativeTarget).Rotator();

	//From is not correct
	FRotator dr = GetOwner()->GetActorRotation() - socketTransformWorld.Rotator();
	FRotator from = FRotationMatrix::MakeFromX(dr.RotateVector(FVector::ForwardVector)).Rotator();
	auto cal = FAimCallibration(from, to);
	return cal;
}

bool USFAimComponent::IsAimingAtTarget(float tolerance) {

	if (!bWasTargetSet) {
		return false;
	}

	FTransform socketTransformWorld = BarrelSocket->GetSocketTransform(SkeletalMesh);
	FRotator barrelRotation = socketTransformWorld.Rotator();
	FRotator targetRotation = FRotationMatrix::MakeFromX(Target - socketTransformWorld.GetLocation()).Rotator();
	FRotator deltaRotator = barrelRotation - targetRotation;

	bool result = deltaRotator.Pitch <= tolerance && deltaRotator.Yaw <= tolerance;
//	UE_LOG(LogTemp, Warning, TEXT("socketTransformWorld : %s targetRotation: %s same %d"), *socketTransformWorld.Rotator().ToString(), *targetRotation.ToString(), result)
	return deltaRotator.Pitch <= tolerance && deltaRotator.Yaw <= tolerance;
}
