// Fill out your copyright notice in the Description page of Project Settings.


#include "SFDialogueManager.h"
#include "Sound/SoundCue.h"
#include "Helpers/LoggingHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
ASFDialogueManager::ASFDialogueManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASFDialogueManager::BeginPlay()
{
	Super::BeginPlay();
	if (!AutoPlayRow.IsNone()) {
		Start(AutoPlayRow);
	}
}

bool ASFDialogueManager::GetDialogueSafe(const FName& Row, FSFDialogueLine*& DialogueLine, AActor*& Speaker)
{
	static const FString ContextStr(TEXT("Player Attack Montage Context"));
	DialogueLine = DialogueDataTable->FindRow<FSFDialogueLine>(Row, ContextStr, true);
	if (!DialogueLine) {
		UE_LOG(LogTemp, Warning, TEXT("'%s' dialogue failed to play for row '%s' no row found."),
			*ULoggingHelper::GetNameOrNull(this),
			*Row.ToString()
		)
		return false;
	}
	if (!DialogueLine->SoundCue) {
		UE_LOG(LogTemp, Error, TEXT("'%s' dialogue failed to play for row '%s' because no sound was set."),
			*ULoggingHelper::GetNameOrNull(this),
			*Row.ToString()
		)
		return false;
	}
	if (!ActorMap.Contains(DialogueLine->Identity)) {
		UE_LOG(LogTemp, Error, TEXT("'%s' dialogue failed to play for row '%s' because invalid identity '%s'"),
			*ULoggingHelper::GetNameOrNull(this),
			*Row.ToString(),
			*DialogueLine->Identity.ToString()
		)
		return false;
	}
	
	Speaker = *ActorMap.Find(DialogueLine->Identity);
	if (!Speaker) {
		UE_LOG(LogTemp, Error, TEXT("'%s' dialogue failed to play Dialogue for row '%s' because Dialogue->Identity '%s' is unassigned in actorMap"),
			*ULoggingHelper::GetNameOrNull(this),
			*Row.ToString(),
			*DialogueLine->Identity.ToString()
		)
		return false;
	}

	return true;
}

void ASFDialogueManager::Start(const FName& Row)
{
	if (!DialogueDataTable) return;
	Stop();

	FSFDialogueLine* Dialogue;
	AActor* Speaker;
	if (!GetDialogueSafe(Row, Dialogue, Speaker)) return;

	UE_LOG(LogTemp, Warning, TEXT("'%s' playing dialogue row '%s'"), 
		*ULoggingHelper::GetNameOrNull(this),
		*Row.ToString())

	CurrentLine = Dialogue;
	CurrentSpeaker = Speaker;

	if (Dialogue->PlaybackDelay == 0.0f) {
		StartDialogueInternal();
	} else {
		GetWorld()->GetTimerManager().SetTimer(AudioDelayTimer, this, &ASFDialogueManager::StartDialogueInternal, Dialogue->PlaybackDelay, false);
	}
}

UAudioComponent* ASFDialogueManager::CreateAudioComponent(USoundWave* SoundWave)
{
	UAudioComponent* AudioComp = NewObject<UAudioComponent>(this, FName("DialogueAudioComponent"));
	if (!AudioComp) { return NULL; }
	AudioComp->RegisterComponent();
	return AudioComp;
}

void ASFDialogueManager::StartDialogueInternal()
{
	if (!CurrentLine || !CurrentSpeaker) return;

	LastPlaybackPercent = 0.0f;

	CurrentAudioComponent = CreateAudioComponent(NULL);
	CurrentAudioComponent->SetWorldLocation(CurrentSpeaker->GetActorLocation());
	CurrentAudioComponent->SetWorldRotation(CurrentSpeaker->GetActorRotation());
	CurrentAudioComponent->Sound = CurrentLine->SoundCue;
	// AudioComp->AttachTo(GetRootComponent(), SocketName, EAttachLocation::KeepWorldPosition);
	CurrentAudioComponent->bAutoDestroy = true;

	if (CurrentLine->DialogueEvents.Num() > 0) {
		CurrentAudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &ASFDialogueManager::OnPlaybackPercent);
	}
	CurrentAudioComponent->OnAudioFinished.AddDynamic(this, &ASFDialogueManager::OnPlaybackFinished);

	CurrentAudioComponent->Play();
}

void ASFDialogueManager::Stop() 
{
	if (CurrentAudioComponent) {
		CurrentAudioComponent->Stop();
		CurrentAudioComponent->OnAudioPlaybackPercent.RemoveDynamic(this, &ASFDialogueManager::OnPlaybackPercent);
		CurrentAudioComponent->OnAudioFinished.RemoveDynamic(this, &ASFDialogueManager::OnPlaybackFinished);
	}
	// TODO fire audio progress finished
}

bool ASFDialogueManager::IsCurrentlyPlaying()
{
	return CurrentLine != NULL && CurrentAudioComponent && CurrentAudioComponent->IsPlaying();
}

// AudioComponent delegate
void ASFDialogueManager::OnPlaybackPercent(const class USoundWave* PlayingSoundWave, float PlaybackPercent)
{
	ExecuteActions(PlaybackPercent);
}

void ASFDialogueManager::ExecuteActions(float PlaybackPercent)
{
	if (!CurrentLine) {
		UE_LOG(LogTemp, Error, TEXT("'%s' PlaybackPercent failed because no CurrentLine"),
			*ULoggingHelper::GetNameOrNull(this)
		)
		return;
	}
	for (FSFDialogueEvent event : CurrentLine->DialogueEvents) {
		if (LastPlaybackPercent <= event.AudioTimePercent && event.AudioTimePercent < PlaybackPercent) {
			OnDialogueAction.Broadcast(event.Name);
		}
	}
	LastPlaybackPercent = PlaybackPercent;
}

void ASFDialogueManager::OnPlaybackFinished()
{
	ExecuteActions(2.0f);
	if (CurrentLine && !CurrentLine->NextLine.IsNone()) {
		Start(CurrentLine->NextLine);
	}
}
