#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFTracker.generated.h"

class AActor;
class USceneComponent;

UENUM()
enum ETrackerType {
	None,
	Component,
	Vector
};

UCLASS(meta = (BlueprintSpawnableComponent), ShowCategories = (Targeting))
class SPACEFORCE_API USFTracker : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere)
	AActor* InitialTrackedActor;

	UFUNCTION(BlueprintCallable)
	void SetTargetVector(FVector Vector);

	UFUNCTION(BlueprintCallable)
	void SetTargetComponent(USceneComponent* Component, bool& OutTargetChanged);

	UFUNCTION(BlueprintCallable)
	void SetTargetActor(AActor* Actor, bool& OutTargetChanged);

	UFUNCTION(BlueprintCallable)
	void ClearTarget();

	UFUNCTION(BlueprintCallable)
	bool GetTarget(FVector& OutPosition, FVector& OutVelocity);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasTarget() const;

public:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<USceneComponent> TrackedComponent;

	UPROPERTY(Transient)
	FVector TrackedVector;

	UPROPERTY(Transient)
	TEnumAsByte<ETrackerType> TrackerType;

};