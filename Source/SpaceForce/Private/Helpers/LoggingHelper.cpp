// Fill out your copyright notice in the Description page of Project Settings.


#include "Helpers/LoggingHelper.h"

FString ULoggingHelper::GetNameOrNull(UObject* Object)
{
	return Object ? Object->GetName() : FString("NULL");
}