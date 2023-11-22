// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;
class UCheckBox;
class UWidgetAnimation;

/**
 * Widget for pause time.
 */
UCLASS()
class PROJECTPUMPKIN_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual void OnReturnToMenuClicked();

	UFUNCTION()
	virtual void OnToggleMusic(bool bPlayMusic);

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnToMenu;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* ToggleMusic;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BlurIn;
};
