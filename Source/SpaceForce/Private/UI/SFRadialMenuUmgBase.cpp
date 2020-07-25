// Fill out your copyright notice in the Description page of Project Settings.


#include "SFRadialMenuUmgBase.h"

void USFRadialMenuUmgBase::SetData(int Number)
{
	NumberOfItems = Number;
	ReceiveOnSetData();
}

void USFRadialMenuUmgBase::SetAxisInput(FVector2D Input)
{
	if (Input.Size() < MinFocusRadius)
		return;

	const float increment = 360.0f / NumberOfItems;
	float angle = FMath::Atan2(Input.Y, Input.X) * 180/PI;
	if (angle < 0)
	{
		angle = 360 + angle;
	}

	float index = (angle / increment);
	UE_LOG(LogTemp, Warning, TEXT("angle = %f, increment = %f, index = %f"), angle, increment, index);

	ReceiveFocusData(index);

}