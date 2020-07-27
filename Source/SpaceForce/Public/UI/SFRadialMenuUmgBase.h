#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFRadialMenuOption.h"
#include "SFRadialMenuUmgBase.generated.h"

UCLASS()
class SPACEFORCE_API USFRadialMenuUmgBase : public UUserWidget
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable)
    void ReloadData();

    UFUNCTION(BlueprintCallable)
    void SetAxisInput(FVector2D Vector);

    UFUNCTION(BlueprintCallable)
    void SetData(TArray<FSFRadialMenuOption> NewData);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    void GetSphericalCoordinatesForIndex(const int& Index, float& OutMax, float& OutMin, float& OutIncrement);

    // how far from the center (0,1) must I go to select something in SetAxisInput for focus to be called
    UPROPERTY(EditAnywhere)
    float MinFocusRadius = 0.2f;

    UFUNCTION(BlueprintPure, BlueprintCallable)
    float GetAngleIncrement() const;

protected:
  
    UPROPERTY(BlueprintReadonly)
    TArray<FSFRadialMenuOption> Data;

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "OnSetData"))
    void ReceiveOnSetData();

// Selected callbacks
protected:

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSelectedIndexChanged"))
    void ReceiveOnSelectedIndexChanged(int NewIndex, int OldIndex);

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFocusedIndexChanged"))
    void ReceiveOnFocusedIndexChanged(int NewIndex, int OldIndex);

private:
    UPROPERTY(BlueprintSetter = SetSelectedIndex, BlueprintGetter = GetSelectedIndex)
    int SelectedIndex;
public:
    UFUNCTION(BlueprintSetter)
    void SetSelectedIndex(int Index);

    UFUNCTION(BlueprintGetter)
    int GetSelectedIndex();

private:
    UPROPERTY(BlueprintSetter = SetFocusedIndex, BlueprintGetter = GetFocusedIndex)
    int FocusedIndex;
protected:
    UFUNCTION(BlueprintSetter)
    void SetFocusedIndex(int Index);
public:
    UFUNCTION(BlueprintGetter)
    int GetFocusedIndex();
};
