// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrameWidget.generated.h"

class USoundBase;
class USoundMix;
class USoundClass;
struct FTimerHandle;

/**
 * Base widget for all user interface compound widgets.
 */
UCLASS(abstract)
class PROJECTPUMPKIN_API UFrameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFrameWidget(const FObjectInitializer& ObjectInitializer);

	void SetPlayMusic(bool bShouldPlay);

	bool IsPlayingMusic() const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	USoundBase* BackgroundMusic;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundMix* MusicMix;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundClass* MusicSoundClass;

private:
	void StartMusicLoop();

	FTimerHandle LoopSoundHandle;

};
