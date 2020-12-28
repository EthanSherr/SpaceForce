#include "Weapons/SFSimpleTurretAnimInstance.h"
#include "Weapons/SFTurretDelegate.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"


USFSimpleTurretAnimInstance::USFSimpleTurretAnimInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USFSimpleTurretAnimInstance::Initialize(FName BarrelNameValue, FName MuzzleNameValue)
{
	USkeletalMeshComponent* SkeletalMesh = GetOwningComponent();
	BarrelSocket = SkeletalMesh->GetSocketByName(BarrelNameValue);
	MuzzleSocket = SkeletalMesh->GetSocketByName(MuzzleNameValue);
	if (!WasInitialized(true)) {
		return;
	}
	InitialBarrelTransformCS = BarrelSocket->GetSocketTransform(SkeletalMesh) * SkeletalMesh->GetComponentTransform().Inverse();
	BarrelLength = FVector::Distance(BarrelSocket->GetSocketLocation(SkeletalMesh), MuzzleSocket->GetSocketLocation(SkeletalMesh));
}

float USFSimpleTurretAnimInstance::GetBarrelLength()
{
	return BarrelLength;
}

bool USFSimpleTurretAnimInstance::WasInitialized(bool logError)
{
	bool wasInit = BarrelSocket != NULL && MuzzleSocket != NULL && GetOwningComponent() != NULL;
	if (!wasInit && logError) {
		UE_LOG(LogTemp, Error, TEXT("Unable to initialize aim component, barrel or muzzle sockets were invalid.  Check your socket names."))
	}
	return wasInit;
}

void USFSimpleTurretAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AActor* OwningActor = GetOwningActor();
	FVector Target;
	if (!OwningActor ||
		!OwningActor->GetClass()->ImplementsInterface(USFTurretDelegate::StaticClass()) ||
		!ISFTurretDelegate::Execute_GetTarget(OwningActor, NULL, Target))
	{
		Yaw = 0.0f;
		Pitch = 0.0f;
		return;
	}

	USkeletalMeshComponent* SkeletalMesh = GetOwningComponent();
	FTransform initialBarrelWS = InitialBarrelTransformCS * SkeletalMesh->GetComponentTransform();
	FVector initialBarrelForwardWS = initialBarrelWS.Rotator().RotateVector(FVector::ForwardVector);
	FTransform socketTransformWS = BarrelSocket->GetSocketTransform(SkeletalMesh);

	FRotator from = socketTransformWS.Rotator() - initialBarrelWS.Rotator();
	FTransform t = socketTransformWS * initialBarrelWS.Inverse();

	FVector relativeTarget = initialBarrelWS.Rotator().UnrotateVector(Target - socketTransformWS.GetTranslation());
	FRotator to = FRotationMatrix::MakeFromX(relativeTarget).Rotator();

	Yaw = to.Yaw;
	Pitch = to.Pitch;

	AimDelta = FMath::Max(to.Yaw - Yaw, to.Pitch - Pitch);
}

float USFSimpleTurretAnimInstance::GetAimDelta() const
{
	return AimDelta;
}