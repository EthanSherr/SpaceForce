// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFTurretActor.generated.h"

class USFTurretComponent;
class USkeletalMeshComponent;

UCLASS()
class SPACEFORCE_API ASFTurretActor : public AActor
{
	GENERATED_UCLASS_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USFTurretComponent* TurretComponent;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName SocketName;

	UFUNCTION(BlueprintCallable)
	void AimAt(FVector Target);

	UFUNCTION(BlueprintCallable)
	void AimAtActor(AActor* TargetActor);

	// UI
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	UMaterialInterface* MaterialIcon;
};
