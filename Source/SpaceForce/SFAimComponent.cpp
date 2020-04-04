// Fill out your copyright notice in the Description page of Project Settings.


#include "SFAimComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

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
	FTransform socketTransform = BarrelSocket->GetSocketTransform(SkeletalMesh);
	FRotator currentRot = socketTransform.GetRotation().Rotator() - GetOwner()->GetActorRotation();
	FTransform t = GetOwner()->GetTransform().Inverse() * socketTransform;

	FVector deltaToBarrel = Target - socketTransform.GetLocation();
	FVector componentSpaceTarget = GetOwner()->GetActorRotation().UnrotateVector(deltaToBarrel);
	FRotator targetRotation = FRotationMatrix::MakeFromX(componentSpaceTarget).Rotator();
	auto callibration = FAimCallibration(currentRot, targetRotation);
	UE_LOG(LogTemp, Warning, TEXT("calibraiton %s"), *callibration.ToString())
	return callibration;
}

