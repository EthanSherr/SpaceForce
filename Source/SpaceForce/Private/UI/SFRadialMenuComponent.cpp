// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRadialMenuComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SFRadialMenuUmgBase.h"

USFRadialMenuComponent::USFRadialMenuComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;\

	ConstructorHelpers::FClassFinder<USFRadialMenuUmgBase> UmgClassName(TEXT("/Game/Blueprints/UI/RadialMenuUmgTest"));
	Widget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("SphereComp"));
	Widget->SetupAttachment(this);
	Widget->SetWidgetClass(UmgClassName.Class);
	Widget->SetRelativeScale3D(FVector::OneVector * 0.2f);
	Widget->SetRelativeRotation(FRotator(0, 180, 0));
	Widget->SetCollisionResponseToAllChannels(ECR_Ignore);
	Widget->SetDrawSize(FVector2D(1, 1) * 1000.0f);

	const float Scale = 0.2f;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	CursorMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CursorComponent"));
	CursorMesh->SetupAttachment(this);
	CursorMesh->SetStaticMesh(MeshAsset.Object);
	CursorMesh->SetRelativeScale3D(FVector::OneVector * Scale);
	CursorMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	CursorRadius = 400.0f * Scale;
	CursorRadiusMax = 350.0f * Scale;
}


// Called when the game starts
void USFRadialMenuComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USFRadialMenuComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USFRadialMenuComponent::OpenMenu(bool bOpen)
{
	bMenuIsOpen = bOpen;
	OnOpenMenu(bOpen);
}

USFRadialMenuUmgBase* USFRadialMenuComponent::GetRadialMenu() const
{
	return Cast<USFRadialMenuUmgBase>(Widget->GetUserWidgetObject());
}

void USFRadialMenuComponent::SetAxisInput(FVector2D Vector)
{
	USFRadialMenuUmgBase* RadialMenu = GetRadialMenu();
	if (RadialMenu)
	{
		RadialMenu->SetAxisInput(Vector);
	}
	FVector Location = FVector(0, Vector.X, Vector.Y) * CursorRadius;
	CursorMesh->SetRelativeLocation(Location,  false);
}

void USFRadialMenuComponent::SelectFocusedIndex()
{
	const auto RadialMenu = GetRadialMenu();
	if (!RadialMenu)
		return;

	const int FocusedIndex = RadialMenu->GetFocusedIndex();
	if (FocusedIndex == -1)
		return;

	RadialMenu->SetSelectedIndex(FocusedIndex);
}