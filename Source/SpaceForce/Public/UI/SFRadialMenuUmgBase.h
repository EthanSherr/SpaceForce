// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFRadialMenuUmgBase.generated.h"

/**
 * 
 */
UCLASS()
class SPACEFORCE_API USFRadialMenuUmgBase : public UUserWidget
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    void SetAxisInput(FVector2D Vector);

    UFUNCTION(BlueprintCallable)
    void SetData(int Number);

    // how far from the center (0,1) must I go to select something in SetAxisInput for focus to be called
    UPROPERTY(EditAnywhere)
    float MinFocusRadius = 0.2f;

protected:
    UPROPERTY(BlueprintReadonly)
    int NumberOfItems;

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "OnSetData"))
    void ReceiveOnSetData();

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFocusData"))
    void ReceiveFocusData(int DataIndex);
};
