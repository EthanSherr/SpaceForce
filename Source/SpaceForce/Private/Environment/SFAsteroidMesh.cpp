#include "SFAsteroidMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "SpaceForce.h"
#include "UObject/ConstructorHelpers.h"
#include "../Components/SFDynamicPrimitiveRegistration.h"
#if WITH_EDITORONLY_DATA
#include "DrawDebugHelpers.h"
#endif

ASFAsteroidMesh::ASFAsteroidMesh(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = AsteroidMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("Asteroid"));
	AsteroidMesh->SetLinearDamping(0.0f);
	AsteroidMesh->SetCollisionProfileName(COLLISION_PROFILE_BLOCK_ALL_DYNAMIC);
	AsteroidMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AsteroidMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Assets/mesh/rocks/SM_Rock_01.SM_Rock_01'"));
	AsteroidMesh->SetStaticMesh(Mesh.Object);

	DirectionTarget = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, FName("VelocityArrow"));
	DirectionTarget->SetupAttachment(RootComponent);

	DynamicPulse = ObjectInitializer.CreateDefaultSubobject<USFDynamicPrimitiveRegistration>(this, FName("DynamicCollision"));

	bActivateOnBegin = false;
	DebugTotalSteps = 25;
	DebugAtTime = DebugTotalSteps / 2.0f;
	
	SegmentSizeInSeconds = 1.0f;

#if WITH_EDITORONLY_DATA
	VisComponent = ObjectInitializer.CreateDefaultSubobject<UAsteroidVisComponent>(this, FName("VIS"));
	VisComponent->SetupAttachment(RootComponent);
#endif
}

void ASFAsteroidMesh::BeginPlay()
{
	Super::BeginPlay();
}

void ASFAsteroidMesh::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetActive(bActivateOnBegin);
}

void ASFAsteroidMesh::SetActive(bool InActivate)
{
	AsteroidMesh->SetSimulatePhysics(InActivate);
	if (InActivate)
	{
		AsteroidMesh->SetPhysicsLinearVelocity(LinearVelocity);
		AsteroidMesh->SetPhysicsAngularVelocityInDegrees(AngularVelocity);
		if (bDynamicCollisionRegistration)
		{
			DynamicPulse->BeginDynamicPulse();
		}
	}
}

void ASFAsteroidMesh::SetVelocity(FVector InVelocity)
{
	FVector Direction = InVelocity.GetSafeNormal();
	float Magnitude = InVelocity.Size();

	FRotator r = FRotationMatrix::MakeFromX(Direction).Rotator();
	DirectionTarget->SetWorldRotation(r);
	DirectionTargetMagnitudeScale = Magnitude;
	LinearVelocity = InVelocity;
}

void ASFAsteroidMesh::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("OnConstruction c++"))
	Super::OnConstruction(Transform);
	LinearVelocity = DirectionTargetMagnitudeScale * DirectionTarget->GetForwardVector();
}

void ASFAsteroidMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("PostEditChangeProperty c++"))
	if (!PropertyChangedEvent.Property)
		return;

	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	UE_LOG(LogTemp, Warning, TEXT("PropertyName %s"), *PropertyName.ToString())

	if (!PropertyChangedEvent.MemberProperty)
		return;

	FName MemberPropertyName = PropertyChangedEvent.MemberProperty->GetFName();
	UE_LOG(LogTemp, Warning, TEXT("MemberPropertyName %s"), *MemberPropertyName.ToString())

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ASFAsteroidMesh::RespondToTrigger_Implementation(class AActor* Source, class ASFPlayerTriggerBox* TriggerBox)
{
	SetActive(true);
}

#if WITH_EDITORONLY_DATA
UAsteroidVisComponent::UAsteroidVisComponent()
{
	bTickInEditor = true;
	PrimaryComponentTick.bStartWithTickEnabled = PrimaryComponentTick.bCanEverTick = true;
}

void UAsteroidVisComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickType == ELevelTick::LEVELTICK_ViewportsOnly)
	{
		if (UWorld* World = GetWorld())
		{
			if (auto* OwnerAsAsteroidMesh = Cast<ASFAsteroidMesh>(GetOwner()))
			{
				if (OwnerAsAsteroidMesh->bKeepDebugDisplayOn || IsOwnerSelected())
				{
					const float Magnitude = OwnerAsAsteroidMesh->LinearVelocity.Size();
					const FVector Direction = OwnerAsAsteroidMesh->LinearVelocity.GetSafeNormal();
					FVector Start = OwnerAsAsteroidMesh->GetActorLocation();
					FVector Current = Start;
					for (int i = 0; i < OwnerAsAsteroidMesh->DebugTotalSteps; i++)
					{
						const FVector End = Direction * Magnitude + Current;
						DrawDebugPoint(World, End, 5, FColor::Green, false, 0.001, 2);
						Current = End;
					}

					float DebugAtTime = OwnerAsAsteroidMesh->DebugAtTime;
					if (DebugAtTime != 0.0f) 
					{
						const FVector Target = Direction * Magnitude * OwnerAsAsteroidMesh->DebugAtTime + Start;
						DrawDebugLine(World, Start, Target, FColor::Red, false, 0.001, 2, 0.5f);
						DrawDebugPoint(World, Target, 7, FColor::Red, false, 0.001, 2);
					}
				}
			}
		}
	}
}
#endif