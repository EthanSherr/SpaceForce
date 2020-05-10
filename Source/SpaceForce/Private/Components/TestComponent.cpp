// Fill out your copyright notice in the Description page of Project Settings.

#include "TestComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UTestComponent::UTestComponent(const FObjectInitializer& Init) : Super(Init)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//MakeSomeOtherComponent();
	// ...
}


// Called when the game starts
void UTestComponent::BeginPlay()
{
	Super::BeginPlay();
	//MakeSomeOtherComponent();
	// ...

}

void UTestComponent::MakeSomeOtherComponent() {
	AActor* Owner = GetOwner();
	USphereComponent* BeginSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
	BeginSphere->SetSphereRadius(30.f);
	BeginSphere->RegisterComponentWithWorld(GetWorld());
	BeginSphere->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	BeginSphere->SetWorldLocation(Owner->GetActorLocation(), false, NULL, ETeleportType::TeleportPhysics);
	BeginSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	BeginSphere->bHiddenInGame = false;
	BeginSphere->SetVisibility(false);
}

void UTestComponent::InitializeComponent() {
	Super::InitializeComponent();
	UE_LOG(LogTemp, Warning, TEXT("Initialize Component!"))
	MakeSomeOtherComponent();
}



// Called every frame
void UTestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

