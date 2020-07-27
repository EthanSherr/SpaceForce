// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRadialMenuUmgBase.h"

void USFRadialMenuUmgBase::SetData(TArray<FSFRadialMenuOption> NewData)
{
	Data = NewData;
	FocusedIndex = -1;
	SelectedIndex = -1;
	ReceiveOnSetData();
}

TArray<FSFRadialMenuOption> USFRadialMenuUmgBase::GetData() const
{
	return Data;
}

void USFRadialMenuUmgBase::ReloadData()
{
	ReceiveOnSetData();
}

float USFRadialMenuUmgBase::GetAngleIncrement() const
{
	return 360.0f / Data.Num();
}

void USFRadialMenuUmgBase::GetSphericalCoordinatesForIndex(const int& Index, float& OutMax, float& OutMin, float& OutIncrement)
{
	OutIncrement = GetAngleIncrement();
	OutMin = OutIncrement * (Index - 0.5f) + 90.0f;
	OutMax = OutMin + OutIncrement;
}

void USFRadialMenuUmgBase::SetAxisInput(FVector2D Input)
{
	if (Input.Size() < MinFocusRadius || Data.Num() == 0)
		return;

	const float Increment = GetAngleIncrement();
	float InputAngle = FMath::Atan2(Input.Y, Input.X) * 180/PI;
	InputAngle = (InputAngle - 90.0f + Increment * 0.5f);
	if (InputAngle < 0)
		InputAngle += 360;

	float NewFocusedIndex = (InputAngle / Increment);
	//UE_LOG(LogTemp, Warning, TEXT("angle = %f, increment = %f, index = %f"), InputAngle, Increment, NewFocusedIndex);
	SetFocusedIndex(NewFocusedIndex);
} 

void USFRadialMenuUmgBase::SetFocusedIndex(int Index)
{
	if (Index == FocusedIndex) 
		return;
	const int OldIndex = FocusedIndex;
	FocusedIndex = Index;
	OnFocusedChanged.Broadcast(this, FocusedIndex, OldIndex);
	//ReceiveOnFocusedIndexChanged(FocusedIndex, OldIndex);
}

int USFRadialMenuUmgBase::GetFocusedIndex()
{
	return FocusedIndex;
}

void USFRadialMenuUmgBase::SetSelectedIndex(int Index)
{
	if (Index == SelectedIndex)
		return;
	const int OldIndex = SelectedIndex;
	SelectedIndex = Index;
	OnSelectedChanged.Broadcast(this, SelectedIndex, OldIndex);
}

int USFRadialMenuUmgBase::GetSelectedIndex()
{
	return SelectedIndex;
}