// Fill out your copyright notice in the Description page of Project Settings.


#include "SFFlightPath.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

ASFFlightPath::ASFFlightPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	Spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("SplinePath"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
}

void ASFFlightPath::OnConstruction(const FTransform& Transform)
{
	SetupSpline();
}

FVector ASFFlightPath::GetLocationAtDistance(float distance)
{
	return Spline->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
}

void ASFFlightPath::SetupSpline()
{
	FVector BeginSphereLocation = Spline->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
	USphereComponent* BeginSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
	BeginSphere->SetSphereRadius(30.f);
	BeginSphere->bHiddenInGame = false;
	BeginSphere->RegisterComponentWithWorld(GetWorld());
	BeginSphere->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	BeginSphere->SetWorldLocation(BeginSphereLocation, false, NULL, ETeleportType::TeleportPhysics);
}

