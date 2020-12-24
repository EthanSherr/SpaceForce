#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFAimVisualization.generated.h"

class ASFTurretActor;

UCLASS( ClassGroup=(Custom), Blueprintable )
class SPACEFORCE_API ASFAimVisualization : public AActor
{
	GENERATED_BODY()

public:	
	ASFAimVisualization();

public:		
	UFUNCTION(BlueprintCallable)
	void SetTargetComponent(USceneComponent* SceneComponent);

	UFUNCTION(BlueprintCallable)
	void SetTargetLocation(FVector Value);

	UFUNCTION(BlueprintCallable)
	void ClearTarget();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetTarget(FVector& OutVector) const;

	UFUNCTION()
	void SetActivated(bool bValue);

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveSetActivated(bool bValue);

	UPROPERTY(BlueprintReadonly)
	ASFTurretActor* Turret;

protected:
	UPROPERTY()
	bool bTargetLocationSet;

	UPROPERTY()
	bool bActivated;

	UPROPERTY()
	FVector Target;

	UPROPERTY()
	USceneComponent* TargetComp;
};
