#include "Player/SFShipDestructible.h"
#include "DestructibleComponent.h"
#include "SpaceForce.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"

ASFShipDestructible::ASFShipDestructible(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	DestructibleComp = ObjectInitializer.CreateDefaultSubobject<UDestructibleComponent>(this, TEXT("DestructibleComp"));
	DestructibleComp->SetSimulatePhysics(true);
	DestructibleComp->SetEnableGravity(true);
	DestructibleComp->SetCollisionProfileName(COLLISION_PROFILE_DESTRUCTIBLE);
	DestructibleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DestructibleComp->SetNotifyRigidBodyCollision(true);
	DestructibleComp->bApplyImpulseOnDamage = false;

	RootComponent = DestructibleComp;

	DestructionSettings.MinDamage = 1.0f;
	DestructionSettings.DamageCapMax = 1.0f;
}

void ASFShipDestructible::Initialize(UPrimitiveComponent* Comp, UDestructibleMesh* DestructibleFacade)
{
	DestructibleComp->SetDestructibleMesh(DestructibleFacade);
	DestructibleComp->MoveIgnoreComponents.Add(Comp);
	DestructibleComp->SetSimulatePhysics(true);
	DestructibleComp->SetPhysicsLinearVelocity(Comp->GetPhysicsLinearVelocity());
	DestructibleComp->SetPhysicsAngularVelocityInRadians(Comp->GetPhysicsAngularVelocityInRadians());
	DestructibleComp->SetNotifyRigidBodyCollision(true);
	DestructibleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DestructibleComp->OnComponentFracture.AddDynamic(this, &ASFShipDestructible::OnFracture);

	/*
//DestructibleComp = NewObject<USFDestructibleComponent>(this, USFDestructibleComponent::StaticClass(), TEXT("DestructibleFacade"));
//DestructibleComp->RegisterComponent();
//DestructibleComp->MoveIgnoreActors.Add(this);
//DestructibleComp->SetDestructibleMesh(DestructibleFacade);
//DestructibleComp->SetWorldTransform(ShipStaticMesh->GetComponentTransform());
//DestructibleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//DestructibleComp->SetNotifyRigidBodyCollision(true);
//DestructibleComp->SetSimulatePhysics(true);
//DestructibleComp->SetEnableGravity(true);
////DestructibleComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//DestructibleComp->SetCollisionProfileName(COLLISION_PROFILE_PAWN);

//DestructibleComp->SetPhysicsLinearVelocity(ShipStaticMesh->GetPhysicsLinearVelocity());
//DestructibleComp->SetPhysicsAngularVelocityInRadians(ShipStaticMesh->GetPhysicsAngularVelocityInRadians());

//DestructibleComp->OnComponentFracture.AddDynamic(this, &ASFShipPawn::OnFracture);
//DestructibleComp->OnComponentHit.AddDynamic(this, &ASFShipPawn::OnCollision);
//

//DestructibleFacade = NULL;

////ShipStaticMesh->SetVisibility(false, true);
//ShipStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//ShipStaticMesh->SetSimulatePhysics(false);
//ShipStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//SetRootComponent(DestructibleComp);
//
//ShipStaticMesh->DestroyComponent();
	*/
}

void ASFShipDestructible::OnFracture(const FVector& HitPoint, const FVector& HitDirection)
{
	if (FractureSystem)
	{
		UNiagaraComponent* NiagaraComp = NewObject<UNiagaraComponent>(this);
		NiagaraComp->SetWorldTransform(FTransform(HitPoint));
		NiagaraComp->SetAsset(FractureSystem);
		NiagaraComp->bAutoActivate = true;
		NiagaraComp->RegisterComponentWithWorld(GetWorld());
	}
}


float ASFShipDestructible::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
	if (DestructionSettings.DamageCapMax > 0)
	{
		Damage = FMath::Min(Damage, DestructionSettings.DamageCapMax);
	}
	Damage = FMath::Max(DestructionSettings.MinDamage, Damage);

	UE_LOG(LogTemp, Warning, TEXT("%s Takes %f"), *GetName(), Damage)

	DestructibleComp->ReceiveComponentDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}
