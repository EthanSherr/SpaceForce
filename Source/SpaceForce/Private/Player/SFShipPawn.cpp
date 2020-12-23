#include "SFShipPawn.h"
#include "Components/AudioComponent.h"
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
#include "Player/SFShipDestructible.h"
#include "Weapons/SFExplosionEffect.h"

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

	EngineAudio = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, FName("EngineAudio"));
	EngineAudio->SetupAttachment(RootComponent);

	FlightMovement = ObjectInitializer.CreateDefaultSubobject<USFSpringFlightMovementComponent>(this, FName("FlightMovement"));
	FlightMovement->LinearMaxSpeed = 0.0f;
	FlightMovement->AngularStiffnessPrimary = 115.0f;
	FlightMovement->AngularStiffnessSecondary = 275.0f;

	HealthComponent = ObjectInitializer.CreateDefaultSubobject<USFHealthComponent>(this, FName("HealthComponent"));
	HealthComponent->Health = 100.0f;

	static ConstructorHelpers::FClassFinder<USFDamageType> ImpactDamageOb(TEXT("/Game/Blueprints/Weapons/BP_ImpactDamage"));
	ImpactDamageType = ImpactDamageOb.Class;

	bDebugCollision = false;
}

void ASFShipPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnInventory();
}

float ASFShipPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	HealthComponent->ChangeHealth(-Damage, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("%s Health is %f"), *GetName(), HealthComponent->Health)

	if (HealthComponent->IsDead() && !bDeathReceived)
	{
		ReceiveDeath_Implementation(Damage, DamageEvent, EventInstigator, DamageCauser);
	}
	return Damage;
}

void ASFShipPawn::ReceiveDeath_Implementation(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (ExplosionEffect)
	{
		FHitResult OutHit;
		FVector OutImpulse;
		DamageEvent.GetBestHitInfo(this, DamageCauser, OutHit, OutImpulse);
		FTransform Transform = FTransform(OutHit.ImpactPoint);
		ASFExplosionEffect* Effect = GetWorld()->SpawnActor<ASFExplosionEffect>(ExplosionEffect, Transform);
	}
	if (DestructibleFacade)
	{
		FlightMovement->ClearTarget();
		ShipStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		FTransform SpawnTransform = ShipStaticMesh->GetComponentTransform();
		ASFShipDestructible* Destructible = GetWorld()->SpawnActorDeferred<ASFShipDestructible>(ASFShipDestructible::StaticClass(), SpawnTransform, this->GetOwner(), this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Destructible->Initialize(ShipStaticMesh, DestructibleFacade);
		Destructible->FractureSystem = FractureSystem;
		Destructible->FinishSpawning(SpawnTransform);
		Destructible->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

		ShipStaticMesh->SetSimulatePhysics(false);
		ShipStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShipStaticMesh->SetVisibility(false, true);
		DestructibleFacade = NULL;
	}

	for (ASFTurretActor* Turret : Turrets)
	{
		Turret->Destroy();
	}
}

void ASFShipPawn::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float ImpulseSize = NormalImpulse.Size();

	float ImpactDamage = ImpulseSize / 50.0f;
	if (ImpactDamage > 25.0f && ImpactDamageType)
	{
		if (ImpactEffect)
		{
			GetWorld()->SpawnActor<ASFExplosionEffect>(ImpactEffect, FTransform(Hit.ImpactPoint));
		}

		FPointDamageEvent PointDmg(ImpactDamage, Hit, NormalImpulse.GetSafeNormal(), ImpactDamageType);
		if (bDebugCollision)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s ImpactDamage %f"), *GetName(), ImpactDamage)
		}
		this->TakeDamage(ImpactDamage, PointDmg, Controller, OtherActor);
	}
}

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
	if (ActiveTurret)
	{
		ActiveTurret->SetActivated(true);
	}
	return ActiveTurret;
}

void ASFShipPawn::TriggerAction(bool bIsPressed)
{
	if (!ActiveTurret)
		return;

	ActiveTurret->TriggerAction(bIsPressed);
}
// end inventory setup