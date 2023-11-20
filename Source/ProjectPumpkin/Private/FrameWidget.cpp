// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameWidget.h"
#include "Sound/SoundBase.h"

void UFrameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackgroundMusic)
	{
		//Using the timer instead looping check in the sound cue to fix possibility of sound being orphaned.
		const float Rate = BackgroundMusic->GetDuration();
		GetWorld()->GetTimerManager().SetTimer(LoopSoundHandle, FTimerDelegate::CreateUObject(this, &UUserWidget::PlaySound, BackgroundMusic), Rate, true, SMALL_NUMBER);
	}
}
