// Fill out your copyright notice in the Description page of Project Settings.

#include "SFPhysMeshGenerator.h"
#include "Components/StaticMeshComponent.h"

void ASFPhysMeshGenerator::InstantiateMesh(const FTransform& Transform, const FVector& Velocity, const FVector& AngularVelocity)
{
	UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	Mesh->RegisterComponentWithWorld(GetWorld());
	Mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	int MeshIndex = Stream.RandRange(0, Templates.Num() - 1);
	Mesh->SetStaticMesh(Templates[MeshIndex]);
	Mesh->SetWorldTransform(Transform, false, NULL, ETeleportType::TeleportPhysics);
	Mesh->SetSimulatePhysics(true);
}