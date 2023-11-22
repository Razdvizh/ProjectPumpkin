// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameWidget.h"
#include "Sound/SoundCue.h"
#include "ProjectPumpkinGameInstance.h"

UFrameWidget::UFrameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UFrameWidget::SetPlayMusic(bool bShouldPlay)
{
	if (BackgroundMusic)
	{
		BackgroundMusic->VolumeMultiplier = bShouldPlay ? 1.f : 0.f;
		if (UProjectPumpkinGameInstance* GameInstance = GetGameInstance<UProjectPumpkinGameInstance>())
		{
			GameInstance->VolumeMultiplier = BackgroundMusic->VolumeMultiplier;
		}
	}
}

bool UFrameWidget::IsPlayingMusic() const
{
	return BackgroundMusic && BackgroundMusic->VolumeMultiplier > 0.f;
}

void UFrameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UProjectPumpkinGameInstance* GameInstance = GetGameInstance<UProjectPumpkinGameInstance>())
	{
		if (BackgroundMusic)
		{
			BackgroundMusic->VolumeMultiplier = GameInstance->VolumeMultiplier;
		}
	}
}
