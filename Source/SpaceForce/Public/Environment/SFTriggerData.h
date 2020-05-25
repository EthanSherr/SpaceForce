#pragma once

//#include "CoreMinimal.h"
//#include "Engine/DataAsset.h"
//#include "SFTriggerData.generated.h"
//
//UENUM(BlueprintType)
//enum ESFTriggerType
//{
//	Default, BehaviorStart, BehaviorEnd, Exit, LevelExit,
//};
//
//UCLASS(BlueprintType)
//class SPACEFORCE_API UTriggerPayload : public UObject
//{
//	GENERATED_UCLASS_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TEnumAsByte<ESFTriggerType> Type;
//
//	UPROPERTY(BlueprintReadOnly, EditAnywhere)
//	FString Level;
//
////#if WITH_EDITOR
////	virtual bool CanEditChange(const UProperty* InProperty) const override;
////#endif
//};
//
//USTRUCT(BlueprintType)
//struct SPACEFORCE_API FSFTriggerData {
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	AActor* Target;
//	
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TEnumAsByte<ESFTriggerType> Type;
//
//	FSFTriggerData() : Target(NULL), Type(ESFTriggerType::Default) {}
//};