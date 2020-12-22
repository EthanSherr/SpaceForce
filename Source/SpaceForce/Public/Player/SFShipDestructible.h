// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFShipDestructible.generated.h"

class UDestructibleComponent;
class UDestructibleMesh;
class UPrimitiveComponent;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct SPACEFORCE_API FSFDestructibleSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageCapMax = 0.0f;
};

UCLASS(Blueprintable)
class SPACEFORCE_API ASFShipDestructible : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFShipDestructible(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditDefaultsOnly)
	FSFDestructibleSettings DestructionSettings;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* FractureSystem;
protected:
	UPROPERTY(EditDefaultsOnly)
	UDestructibleComponent* DestructibleComp;
	
	UFUNCTION()
	void OnFracture(const FVector& HitPoint, const FVector& HitDirection);

public:	

	virtual void Initialize(UPrimitiveComponent* Comp, UDestructibleMesh* DestructibleFacade);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);
};
