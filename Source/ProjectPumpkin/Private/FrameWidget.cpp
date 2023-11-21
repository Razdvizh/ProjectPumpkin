// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameWidget.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UFrameWidget::UFrameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UFrameWidget::SetPlayMusic(bool bShouldPlay)
{
	float VolumeMultiplier;
	if (bShouldPlay)
	{
		VolumeMultiplier = 1.f;
		StartMusicLoop();
	}
	else
	{
		VolumeMultiplier = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(LoopSoundHandle);
	}

	UWorld* World = GetWorld();
	UGameplayStatics::PopSoundMixModifier(World, MusicMix);
	UGameplayStatics::SetSoundMixClassOverride(World, MusicMix, MusicSoundClass, VolumeMultiplier);
	UGameplayStatics::PushSoundMixModifier(World, MusicMix);
}

bool UFrameWidget::IsPlayingMusic() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(LoopSoundHandle);
}

void UFrameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartMusicLoop();

}

void UFrameWidget::StartMusicLoop()
{
	if (GEngine->UseSound() && BackgroundMusic)
	{
		//Using the timer instead looping check in the sound cue to fix possibility of sound being orphaned.
		const float Rate = BackgroundMusic->GetDuration();
		GetWorld()->GetTimerManager().SetTimer(LoopSoundHandle, FTimerDelegate::CreateUObject(this, &UUserWidget::PlaySound, BackgroundMusic), Rate, true, SMALL_NUMBER);
	}
}
