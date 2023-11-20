// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrameWidget.h"
#include "MainMenuWidget.generated.h"

class UWorld;
class UGameGuideWidget;
class UCreditsWidget;
class UButton;
class UWidgetAnimation;

/**
 * Widget that represents main menu interface.
 */
UCLASS()
class PROJECTPUMPKIN_API UMainMenuWidget : public UFrameWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable, Category = "Menu Widget")
	virtual void OnStartPlayClicked();

	UFUNCTION(BlueprintCallable, Category = "Menu Widget")
	virtual void OnHowToPlayClicked();

	UFUNCTION(BlueprintCallable, Category = "Menu Widget")
	virtual void OnCreditsClicked();

	UFUNCTION(BlueprintCallable, Category = "Menu Widget")
	virtual void OnExitGameClicked();

	UFUNCTION(BlueprintCallable, Category = "Menu Widget")
	virtual void OnReturnToMenuClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TSoftObjectPtr<UWorld> GameLevel;

	UPROPERTY(meta = (BindWidget))
	UGameGuideWidget* GameGuide;

	UPROPERTY(meta = (BindWidget))
	UCreditsWidget* CreditsSection;

	UPROPERTY(meta = (BindWidget))
	UButton* StartPlay;

	UPROPERTY(meta = (BindWidget))
	UButton* HowToPlay;

	UPROPERTY(meta = (BindWidget))
	UButton* Credits;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitGame;

	UPROPERTY(meta = (BindWidget))
	UButton* ReturnToMenu;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Transition;

};
