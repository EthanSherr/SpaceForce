// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SFLevelDiorama.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelSelected, ASFLevelDiorama*, Diorama);

UCLASS()
class SPACEFORCE_API ASFLevelDiorama : public AActor
{
	GENERATED_UCLASS_BODY()

protected:

	UPROPERTY(EditAnywhere)
	USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetLevel)
	FName Level;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFocusChanged"))
	void ReceiveOnFocusChanged(bool IsFocused);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSelectedChanged"))
	void ReceiveOnSelectedChanged(bool IsSelected);

private:
	UPROPERTY(BlueprintSetter = SetSelected, BlueprintGetter = GetSelected)
	bool bSelected;

	UPROPERTY(BlueprintSetter = SetFocused, BlueprintGetter = GetFocused)
	bool bFocused;
public:
	UFUNCTION(BlueprintGetter)
	FName GetLevel();

	UPROPERTY(BlueprintAssignable)
	FOnLevelSelected OnLevelSelected;
	
	UFUNCTION(BlueprintGetter)
	bool GetFocused();

	UFUNCTION(BlueprintGetter)
	bool GetSelected();

	UFUNCTION(BlueprintSetter)
	void SetSelected(bool InSelected);

	UFUNCTION(BlueprintSetter)
	void SetFocused(bool InFocused);
};
