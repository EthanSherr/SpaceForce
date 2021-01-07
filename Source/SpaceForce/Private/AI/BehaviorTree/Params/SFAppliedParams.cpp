#include "AI/BehaviorTree/Params/SFAppliedParams.h"

USFAppliedParams::USFAppliedParams(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bApplyAtBegin = true;
}

bool USFAppliedParams::Apply_Implementation(APawn* Bot, AAIController* Controller)
{
	UE_LOG(LogTemp, Warning, TEXT("SFAppliedParam Apply_Implementation has no default implemmentation"))
	return false;
}