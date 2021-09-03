// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "SFDialogueManager.generated.h"

class USoundCue;
class UDataTable;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueAction, FName, EventName);

UENUM(BlueprintType)
enum class ETimeType : uint8
{
	Percent     UMETA(DisplayName = "Percent"),
	Second      UMETA(DisplayName = "Second")
};

USTRUCT(BlueprintType)
struct SPACEFORCE_API FSFDialogueEvent : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWRite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float AudioTimePercent;



	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//ETimeType Type;
};

USTRUCT(BlueprintType)
struct SPACEFORCE_API FSFDialogueLine : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanSkip = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Identity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Text;

	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundCue* SoundCue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName NextLine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PlaybackDelay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSFDialogueEvent> DialogueEvents;
};

UCLASS()
class SPACEFORCE_API ASFDialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASFDialogueManager();

	/** This will auto-play from specific row in DialogueDataTable on BeginPlay*/
	UPROPERTY(EditAnywhere)
	FName AutoPlayRow;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Initialization")
	TMap<FName, AActor*> ActorMap;

	UPROPERTY(EditAnywhere, Category = "Initialization")
	UDataTable* DialogueDataTable;

public:
	UFUNCTION(BlueprintCallable, Category = "DialogueManager")
	void Start(const FName& Row);

	UFUNCTION(BlueprintCallable, Category = "DialogueManager")
	bool IsCurrentlyPlaying();

	UPROPERTY(BlueprintAssignable, Category = "DialogueManager")
	FOnDialogueAction OnDialogueAction;

protected:

	// Current Playing audio state.
	FSFDialogueLine* CurrentLine;

	UPROPERTY(Transient)
	AActor* CurrentSpeaker;

	UPROPERTY()
	UAudioComponent* CurrentAudioComponent;

	UPROPERTY(Transient)
	float LastPlaybackPercent;

	UPROPERTY(Transient)
	FTimerHandle AudioDelayTimer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool GetDialogueSafe(const FName& Row, FSFDialogueLine*& DialogueLine, AActor*& Speaker);

	void Stop();


	// AudioComponent delegate
	UFUNCTION()
	void OnPlaybackFinished();

	UFUNCTION()
	void OnPlaybackPercent(const class USoundWave* PlayingSoundWave, float PlaybackPercent);

private:
	UAudioComponent* CreateAudioComponent(class USoundWave* Wave);
	void StartDialogueInternal();

	void ExecuteActions(float PlaybackPercent);
};
