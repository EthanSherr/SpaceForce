// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SFRadialMenuOption.h"
#include "SFRadialMenuComponent.generated.h"

class UWidgetComponent;
class UUserWidget;
class USFRadialMenuUmgBase;
class USFHandController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMenuItemSelected, USFRadialMenuComponent*, Menu, FSFRadialMenuOption, Option, int, Index);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEFORCE_API USFRadialMenuComponent : public USceneComponent
{
	GENERATED_UCLASS_BODY()
public:

	UFUNCTION()
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadonly)
	USFHandController* HandController;

	UFUNCTION(BlueprintCallable)
	void SetData(TArray<FSFRadialMenuOption> Data);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FSFRadialMenuOption> GetData();

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

	UFUNCTION(BlueprintCallable)
	void SelectFocusedIndex();

	UFUNCTION(BlueprintCallable)
		void SetSelectedIndex(int SelectedIndex);

	UPROPERTY(BlueprintReadonly)
	USceneComponent* LookAt;

private:
	UPROPERTY()
	float DrawSizeDim = 500.0f;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetDrawSize() { return DrawSizeDim; }

	UPROPERTY(BlueprintAssignable)
	FOnMenuItemSelected OnMenuItemSelected;

	UFUNCTION(BlueprintCallable)
	void PlayHapticEffect();

	UPROPERTY(BlueprintReadWrite)
	UHapticFeedbackEffect_Base* UIHapticEffect;

public:
	UFUNCTION()
	void OnSelectedChanged(USFRadialMenuUmgBase* Menu, int New, int Old);

	UFUNCTION()
	void OnFocusedChanged(USFRadialMenuUmgBase* Menu, int New, int Old);

protected:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
