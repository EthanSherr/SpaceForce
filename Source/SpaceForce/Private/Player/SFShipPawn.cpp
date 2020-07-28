// Fill out your copyright notice in the Description page of Project Settings.


#include "SFShipPawn.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/SFBoosterManagerComponent.h"
#include "../Components/SFSpringFlightMovementComponent.h"
#include "../Components/SFSplineMovementComponent.h"
#include "../Components/SFHealthComponent.h"
#include "../Environment/SFFlightPath.h"
#include "SFPilotPawn.h"
#include "SpaceForce.h"

ASFShipPawn::ASFShipPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	ShipStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("ShipStaticMesh"));
	ShipStaticMesh->SetSimulatePhysics(true);
	ShipStaticMesh->SetCollisionProfileName(COLLISION_PROFILE_PAWN);
	ShipStaticMesh->OnComponentHit.AddDynamic(this, &ASFShipPawn::OnCollision);
	ShipStaticMesh->SetNotifyRigidBodyCollision(true);

	RootComponent = ShipStaticMesh;

	FlightMovement = ObjectInitializer.CreateDefaultSubobject<USFSpringFlightMovementComponent>(this, FName("FlightMovement"));
	FlightMovement->LinearMaxSpeed = 0.0f;
	FlightMovement->AngularStiffnessPrimary = 115.0f;
	FlightMovement->AngularStiffnessSecondary = 275.0f;

	BoosterManagerComponent = ObjectInitializer.CreateDefaultSubobject<USFBoosterManagerComponent>(this, FName("BoosterManagerComponent"));

	HealthComponent = ObjectInitializer.CreateDefaultSubobject<USFHealthComponent>(this, FName("HealthComponent"));
	HealthComponent->Health = 100.0f;
}

void ASFShipPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HealthComponent->OnDeath.AddDynamic(this, &ASFShipPawn::OnDeath);
}

void ASFShipPawn::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString Name = OtherActor ? OtherActor->GetName() : FString("NULL");
	if (OtherActor && !OtherActor->IsRootComponentMovable())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ship %s hit %s"), *GetName(), *Name)
		float PointDamage = 100.0f;
		TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();
		FPointDamageEvent PointDmg(PointDamage, Hit, Hit.ImpactNormal, DamageType);
		this->TakeDamage(PointDamage, PointDmg, Controller, this);
	}
}


void ASFShipPawn::OnDeath(float Health, float MaxHealth) {
	FlightMovement->ClearTarget();
}

bool ASFShipPawn::IsAlive()
{
	return HealthComponent->IsAlive();
}

ASFPilotPawn* ASFShipPawn::GetOwnerPilot() {
	return Cast<ASFPilotPawn>(GetOwner());
}

float ASFShipPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	HealthComponent->ChangeHealth(-Damage);
	return Damage;
}

USFSplineMovementComponent* ASFShipPawn::GetAssociatedSplineMovementComponent() {
	if (AssociatedSplineMovementComponent) {
		return AssociatedSplineMovementComponent;
	}
	auto Pilot = GetOwnerPilot();
	if (Pilot) {
		AssociatedSplineMovementComponent = Pilot->SplineMovement;
		return AssociatedSplineMovementComponent;
	}
	auto TargetComponent = FlightMovement->TargetComponent;
	if (TargetComponent) {
		auto TargetOwner = TargetComponent->GetOwner();
		AssociatedSplineMovementComponent = Cast<USFSplineMovementComponent>(TargetOwner->GetComponentByClass(USFSplineMovementComponent::StaticClass()));
		if (AssociatedSplineMovementComponent) {
			return AssociatedSplineMovementComponent;
		}
	}
	AssociatedSplineMovementComponent = Cast<USFSplineMovementComponent>(GetComponentByClass(USFSplineMovementComponent::StaticClass()));
	return AssociatedSplineMovementComponent;
}

void ASFShipPawn::TrySetIsBoosting(bool bNewIsBoosting)
{
	BoosterManagerComponent->TrySetIsBoosting(bNewIsBoosting);
}

void ASFShipPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}