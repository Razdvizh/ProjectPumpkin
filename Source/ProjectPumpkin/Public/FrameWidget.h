// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrameWidget.generated.h"

class USoundCue;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	USoundCue* BackgroundMusic;

};
