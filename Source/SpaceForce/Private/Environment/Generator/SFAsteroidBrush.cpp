#include "SFAsteroidBrush.h"
#include "Engine/World.h"
#include "../SFAsteroidMesh.h"

ASFAsteroidBrush::ASFAsteroidBrush(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bGenerateOnConstruction = false;
	MinVelocityMagnitude = 250.0f;
	MaxVelocityMagnitude = 300.0f;
	VariationDegrees = 5.0f;
}

void ASFAsteroidBrush::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!PropertyChangedEvent.Property)
		return;

	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASFAsteroidBrush, bCheckToGenerate))
	{
		if (bCheckToGenerate)
		{
			GenerateTemplates();
		}
		else
		{
			for (auto* Actor : GeneratedActors)
			{
				Actor->Destroy();
			}
			GeneratedActors.Empty();
		}
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASFAsteroidBrush, bDetachGenerated))
	{
		GeneratedActors.Empty();
	}
}

void ASFAsteroidBrush::GeneratePhysics_Implementation(FVector& OutVelocity, FVector& OutAngularVelocity)
{
	const float Magnitude = Stream.FRandRange(MinVelocityMagnitude, MaxVelocityMagnitude);
	FRotator Rotation = FRotator(
		Stream.FRandRange(-VariationDegrees, VariationDegrees),
		Stream.FRandRange(-VariationDegrees, VariationDegrees),
		Stream.FRandRange(-VariationDegrees, VariationDegrees));

	OutVelocity = Rotation.RotateVector(VelocityDirection) * Magnitude;
}

void ASFAsteroidBrush::InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = nullptr;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = nullptr;

	ASFAsteroidMesh* Asteroid = GetWorld()->SpawnActor<ASFAsteroidMesh>(SpawnInfo);
	Asteroid->SetActorTransform(Transform);
	Asteroid->SetVelocity(Velocity);
	GeneratedActors.Add(Asteroid);
}

