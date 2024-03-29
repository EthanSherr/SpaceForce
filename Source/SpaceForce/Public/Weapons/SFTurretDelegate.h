#pragma once

#include "CoreMinimal.h"
#include "SFTurretDelegate.generated.h"

class ASFTurretActor;
class ASFProjectile;

UINTERFACE(BlueprintType)
class SPACEFORCE_API USFTurretDelegate : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SPACEFORCE_API ISFTurretDelegate
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Turret Delegate")
	bool GetTarget(ASFTurretActor* Turret, float DeltaSeconds, FVector& OutTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Turret Delegate")
	bool ProjectileSpawned(ASFTurretActor* Turret, ASFProjectile* Projectile);
};
