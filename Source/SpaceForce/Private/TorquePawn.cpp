// Fill out your copyright notice in the Description page of Project Settings.


#include "TorquePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "SpaceForce.h"

// Sets default values
ATorquePawn::ATorquePawn(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = SkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
	SkeletalMesh->SetCollisionProfileName(COLLISION_PROFILE_PAWN);
	SkeletalMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ATorquePawn::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ATorquePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CMStart = SkeletalMesh->GetCenterOfMass();
	FVector OriginSTart = SkeletalMesh->GetComponentLocation();
	FVector Forward = FVector::ForwardVector; // SkeletalMesh->GetForwardVector();
	DrawDebugPoint(GetWorld(), CMStart, 1, FColor::Green, false, 0, 1);
	DrawDebugLine(GetWorld(), CMStart, CMStart + Forward * 200, FColor::Green, false, 0, 3, 0.5);
	DrawDebugLine(GetWorld(), OriginSTart, OriginSTart + Forward * 200, FColor::Red, false, 0, 1, 1);
	auto Tensor = SkeletalMesh->GetInertiaTensor();
	//Tensor X=1230858.625 Y=1230858.500 Z=742892.375

		//Torque = I * w
		auto T = AngularAcceleration;// SkeletalMesh->GetComponentRotation().UnrotateVector(AngularAcceleration * Tensor);

	//auto T = SkeletalMesh->ScaleByMomentOfInertia(Torque);

	UE_LOG(LogTemp, Warning, TEXT("Tensor %s T %s"), *Tensor.ToString(), *T.ToString())
	SkeletalMesh->AddTorqueInRadians(T, FName(), true);
	
	UE_LOG(LogTemp, Warning, TEXT("%s angular velocity %s"), *GetName(), *SkeletalMesh->GetPhysicsAngularVelocityInRadians().ToString())
	//Tensor X=71787.094 Y=22917.242 Z=62477.676
}

// Called to bind functionality to input
void ATorquePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

