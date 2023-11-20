// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FrameWidget.generated.h"

class USoundBase;
struct FTimerHandle;

/**
 * Base widget for all user interface compound widgets.
 */
UCLASS(abstract)
class PROJECTPUMPKIN_API UFrameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	USoundBase* BackgroundMusic;

private:
	FTimerHandle LoopSoundHandle;

};
