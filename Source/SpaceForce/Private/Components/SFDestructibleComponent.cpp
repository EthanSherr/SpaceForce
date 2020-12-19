// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SFDestructibleComponent.h"
#include "Weapons/SFDamageType.h"
#include "Helpers/LoggingHelper.h"

USFDestructibleComponent::USFDestructibleComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USFDestructibleComponent::ReceiveComponentDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("%s destructible component taking damage from %s"), *GetName(), *ULoggingHelper::GetNameOrNull(DamageCauser))
	USFDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<USFDamageType>() : GetDefault<USFDamageType>();
	Super::ReceiveComponentDamage(DamageTypeCDO->DestructibleDamage, DamageEvent, EventInstigator, DamageCauser);
}