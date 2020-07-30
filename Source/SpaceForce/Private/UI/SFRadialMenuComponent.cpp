// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRadialMenuComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SFRadialMenuUmgBase.h"
#include "../Player/SFHandController.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

USFRadialMenuComponent::USFRadialMenuComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	const float Scale = 0.05f;
	ConstructorHelpers::FClassFinder<USFRadialMenuUmgBase> UmgClassName(TEXT("/Game/Blueprints/UI/BP_RadialMenu"));
	Widget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("SphereComp"));
	Widget->SetupAttachment(this);
	Widget->SetWidgetClass(UmgClassName.Class);
	Widget->SetRelativeScale3D(FVector::OneVector * Scale);
	Widget->SetRelativeRotation(FRotator(0, 180, 0));
	Widget->SetCollisionResponseToAllChannels(ECR_Ignore);
	Widget->SetDrawSize(FVector2D(1, 1) * DrawSizeDim);

	const float CursorScale = 0.05f * Scale;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	CursorMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CursorComponent"));
	CursorMesh->SetupAttachment(this);
	CursorMesh->SetStaticMesh(MeshAsset.Object);
	CursorMesh->SetRelativeScale3D(FVector::OneVector * CursorScale);
	CursorMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	CursorRadius = DrawSizeDim/2 * Scale;
}

void USFRadialMenuComponent::BeginPlay()
{
	Super::BeginPlay();
	auto RadialMenu = GetRadialMenu();
	if (!RadialMenu)
	{
		UE_LOG(LogTemp, Error, TEXT("NO RADIAL MENU ON BEGINPLAY"))
		return;
	}

	RadialMenu->OnSelectedChanged.AddDynamic(this, &USFRadialMenuComponent::OnSelectedChanged);
	RadialMenu->OnFocusedChanged.AddDynamic(this, &USFRadialMenuComponent::OnFocusedChanged);
	RadialMenu->SetDrawSizeDim(DrawSizeDim);
}

void USFRadialMenuComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!LookAt)
		return;

	const FRotator Rotation = (-LookAt->GetComponentLocation() + this->GetComponentLocation()).Rotation();
	SetWorldRotation(Rotation);
}

void USFRadialMenuComponent::SetData(TArray<FSFRadialMenuOption> Data)
{
	GetRadialMenu()->SetData(Data);
}

TArray<FSFRadialMenuOption> USFRadialMenuComponent::GetData()
{
	return GetRadialMenu()->GetData();
}

void USFRadialMenuComponent::OnSelectedChanged(USFRadialMenuUmgBase* Menu, int New, int Old)
{
	PlayHapticEffect();
	OnMenuItemSelected.Broadcast(this, Menu->GetData()[New], New);
}

void USFRadialMenuComponent::OnFocusedChanged(USFRadialMenuUmgBase* Menu, int New, int Old)
{
	PlayHapticEffect();
}

void USFRadialMenuComponent::OpenMenu(bool bOpen)
{
	if (GetData().Num() == 0)
		return;
	bMenuIsOpen = bOpen;
	OnOpenMenu(bOpen);
}

USFRadialMenuUmgBase* USFRadialMenuComponent::GetRadialMenu() const
{
	auto Menu = Cast<USFRadialMenuUmgBase>(Widget->GetUserWidgetObject());
	if (!Menu)
	{
		Widget->InitWidget();
		Menu = Cast<USFRadialMenuUmgBase>(Widget->GetUserWidgetObject());
	}
	return Menu;
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

void USFRadialMenuComponent::PlayHapticEffect()
{
	if (!UIHapticEffect || !HandController) { return; }
	auto PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner) { return; }
	auto PC = PawnOwner->GetController<APlayerController>();
	if (!PC) { return; }
	PC->PlayHapticEffect(UIHapticEffect, HandController->HandTypeFromMotionSource(), 1.0f, false);
}