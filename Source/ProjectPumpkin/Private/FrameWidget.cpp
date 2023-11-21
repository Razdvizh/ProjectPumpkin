// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UFrameWidget::UFrameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UFrameWidget::SetPlayMusic(bool bShouldPlay)
{
	if (BackgroundMusic)
	{
		BackgroundMusic->VolumeMultiplier = bShouldPlay ? 1.f : 0.f;
	}
}

bool UFrameWidget::IsPlayingMusic() const
{
	return BackgroundMusic && BackgroundMusic->VolumeMultiplier > 0.f;
}

void UFrameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlaySound(BackgroundMusic);
}
