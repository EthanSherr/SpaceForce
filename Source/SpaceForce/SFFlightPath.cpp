// Fill out your copyright notice in the Description page of Project Settings.


#include "SFFlightPath.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"

ASFFlightPath::ASFFlightPath(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Spline = ObjectInitializer.CreateDefaultSubobject<USplineComponent>(this, TEXT("SplinePath"));
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	Mesh = MeshAsset.Object;
	splineForward = ESplineMeshAxis::Z;
	bucketSize = 50.0f;
	columns = 30;
	rows = 30;
}

// Called when the game starts or when spawned
void ASFFlightPath::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASFFlightPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASFFlightPath::OnConstruction(const FTransform& Transform)
{
	SetupSpline();
}

void ASFFlightPath::SetupSpline()
{
	UE_LOG(LogTemp, Warning, TEXT("SetupSpline"));

	if (Mesh == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Mesh is null, not constructing VoxelPath."))
			return;
	}

	//for (int32 i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++) {
	//	FVector LocationStart, TangentStart, LocationEnd, TangentEnd;
	//	Spline->GetLocalLocationAndTangentAtSplinePoint(i, LocationStart, TangentStart);
	//	Spline->GetLocalLocationAndTangentAtSplinePoint(i + 1, LocationEnd, TangentEnd);
	//	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	//	SplineMesh->SetStaticMesh(Mesh);
	//	SplineMesh->RegisterComponentWithWorld(GetWorld());
	//	SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	//	SplineMesh->SetMobility(EComponentMobility::Movable);
	//	SplineMesh->SetForwardAxis(splineForward, true);
	//	SplineMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
	//	SplineMesh->SetStartAndEnd(LocationStart, TangentStart, LocationEnd, TangentEnd, true);
	//}

	//float distance = 0;
	//while (distance < Spline->GetSplineLength())
	//{
	//	FVector Position = Spline->GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World);
	//	FVector Forward = Spline->GetTangentAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World).GetSafeNormal();
	//	FVector Right = Spline->GetRightVectorAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World).GetSafeNormal();
	//	FVector Up = Spline->GetUpVectorAtDistanceAlongSpline(distance, ESplineCoordinateSpace::World).GetSafeNormal();

	//	for (int c = 0; c < columns; c++) {
	//		for (int r = 0; r < rows; r++) {
	//			FVector Location = Position + Right * (bucketSize * c) + Up * (bucketSize * r);
	//			//DrawDebugPoint(GetWorld(), Location, 30, FColor(255, 0, 0), true);
	//			DrawDebugLine(GetWorld(), Position, Position + Up * 50, FColor::Emerald, true, -1, 0, 10);
	//			DrawDebugLine(GetWorld(), Position, Position + Right * 50, FColor::Red, true, -1, 0, 10);
	//			//DrawDebugSphere(GetWorld(), Location, 200, 26, FColor(0, 0, 255), true, -1, 0, 2);
	//		}
	//	}

	//	distance += bucketSize;
	//}


}

