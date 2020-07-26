// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SFRadialMenuComponent.generated.h"

class UWidgetComponent;
class UUserWidget;
class USFRadialMenuUmgBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFRadialMenuComponent : public USceneComponent
{
	GENERATED_UCLASS_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void OpenMenu(bool bOpen);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenMenu(bool bOpen);

	UPROPERTY(BlueprintReadonly)
	bool bMenuIsOpen;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* Widget;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	USFRadialMenuUmgBase* GetRadialMenu() const;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CursorMesh;

	UFUNCTION(BlueprintCallable)
	void SetAxisInput(FVector2D Vector);

	UPROPERTY()
	float CursorRadius;

	UPROPERTY()
	float CursorRadiusMax;

	UFUNCTION(BlueprintCallable)
	void SelectFocusedIndex();
};
