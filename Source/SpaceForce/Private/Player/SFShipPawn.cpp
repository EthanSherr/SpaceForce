// Fill out your copyright notice in the Description page of Project Settings.


#include "SFShipPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SFBoosterManagerComponent.h"
#include "Components/SFSpringFlightMovementComponent.h"
#include "Components/SFSplineMovementComponent.h"
#include "Components/SFHealthComponent.h"
#include "Components/SFDestructibleComponent.h"
#include "Environment/SFFlightPath.h"
#include "SpaceForce.h"
#include "Weapons/SFTurretActor.h"
#include "DestructibleComponent.h"
#include "DrawDebugHelpers.h"
#include "Helpers/LoggingHelper.h"
#include "Weapons/SFDamageType.h"

ASFShipPawn::ASFShipPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	// how to replace root https://answers.unrealengine.com/questions/538332/replace-root-component-on-an-inherited-class.html
	ShipStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("ShipStaticMesh"));
	ShipStaticMesh->SetSimulatePhysics(true);
	ShipStaticMesh->SetCollisionProfileName(COLLISION_PROFILE_PAWN);
	ShipStaticMesh->OnComponentHit.AddDynamic(this, &ASFShipPawn::OnCollision);
	ShipStaticMesh->SetNotifyRigidBodyCollision(true);
	ShipStaticMesh->SetEnableGravity(false);
	ShipStaticMesh->bApplyImpulseOnDamage = false;

	RootComponent = ShipStaticMesh;

	FlightMovement = ObjectInitializer.CreateDefaultSubobject<USFSpringFlightMovementComponent>(this, FName("FlightMovement"));
	FlightMovement->LinearMaxSpeed = 0.0f;
	FlightMovement->AngularStiffnessPrimary = 115.0f;
	FlightMovement->AngularStiffnessSecondary = 275.0f;

	HealthComponent = ObjectInitializer.CreateDefaultSubobject<USFHealthComponent>(this, FName("HealthComponent"));
	HealthComponent->Health = 100.0f;
}

void ASFShipPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ASFShipPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	HealthComponent->OnDeath.AddDynamic(this, &ASFShipPawn::OnDeath);
	SpawnInventory();
}

float ASFShipPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	Damage = -HealthComponent->ChangeHealth(-Damage, DamageEvent, EventInstigator, DamageCauser);
	if (HealthComponent->IsDead() && DestructibleFacade)
	{
		DestructibleComp = NewObject<USFDestructibleComponent>(this, USFDestructibleComponent::StaticClass(), TEXT("DestructibleFacade"));
		DestructibleComp->RegisterComponent();
		DestructibleComp->SetDestructibleMesh(DestructibleFacade);
		DestructibleComp->SetWorldTransform(ShipStaticMesh->GetComponentTransform());
		DestructibleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DestructibleComp->SetNotifyRigidBodyCollision(true);
		DestructibleComp->SetSimulatePhysics(true);
		DestructibleComp->SetEnableGravity(true);
		DestructibleComp->SetCollisionProfileName(COLLISION_PROFILE_PAWN);

		DestructibleComp->SetPhysicsLinearVelocity(ShipStaticMesh->GetPhysicsLinearVelocity());
		DestructibleComp->SetPhysicsAngularVelocity(ShipStaticMesh->GetPhysicsAngularVelocity());

		DestructibleComp->OnComponentHit.AddDynamic(this, &ASFShipPawn::OnCollision);

		DestructibleFacade = NULL;

		ShipStaticMesh->SetVisibility(false, true);
		ShipStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SetRootComponent(DestructibleComp);
	}

	if (DestructibleComp)
	{
		//override by damage event
		DestructibleComp->ReceiveComponentDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	}
	return Damage;
}

void ASFShipPawn::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FString Name = OtherActor ? OtherActor->GetName() : FString("NULL");
	if (HitComponent == DestructibleComp)
	{
		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 4, FColor::Red, true, 10, 3);
	} else if (OtherActor && !OtherActor->IsRootComponentMovable())
	{
		UE_LOG(LogTemp, Warning, TEXT("Ship %s hit %s"), *GetName(), *Name)
		float PointDamage = 100.0f;
		TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();
		FPointDamageEvent PointDmg(PointDamage, Hit, Hit.ImpactNormal, DamageType);
		this->TakeDamage(PointDamage, PointDmg, Controller, this);
	}
}

void ASFShipPawn::OnDeath(USFHealthComponent* HealthComp, float Damage) 
{
	FlightMovement->ClearTarget();
}

//void ASFShipPawn::TakeDestructibleDamage(struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
//{
//	if (!DestructibleComp)
//	{
//		UE_LOG(LogTemp, Error, TEXT("%s ReceiveDestructibleDamage called but !DestructibleComp"), *GetName())
//		return;
//	}
//	USFDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<USFDamageType>() : GetDefault<USFDamageType>();
//	DestructibleComp->ReceiveComponentDamage(DamageTypeCDO->DestructibleDamage, DamageEvent, EventInstigator, DamageCauser);
//}

UPrimitiveComponent* ASFShipPawn::GetRootPrimitive() const
{
	return Cast<UPrimitiveComponent>(GetRootComponent());
}

bool ASFShipPawn::IsAlive()
{
	return HealthComponent->IsAlive();
}
// inventory setup
void ASFShipPawn::SpawnInventory()
{
	for (int32 i = 0; i < TurretClasses.Num(); i++)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ASFTurretActor* NewTurret = GetWorld()->SpawnActor<ASFTurretActor>(TurretClasses[i], SpawnInfo);
		AddTurret(NewTurret);
	}
}

void ASFShipPawn::AddTurret(ASFTurretActor* Turret)
{
	if (!Turret)
		return;
	Turrets.Add(Turret);
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Turret->AttachToActor(this, AttachmentRules, Turret->SocketName);
	Turret->SetOwner(this);
}

ASFTurretActor* ASFShipPawn::ActivateTurret(int Index)
{
	ActiveTurret = Turrets[Index];
	return ActiveTurret;
}

void ASFShipPawn::TriggerAction(bool bIsPressed)
{
	if (!ActiveTurret)
		return;

	ActiveTurret->TriggerAction(bIsPressed);
}
// end inventory setup