// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "GameGuideWidget.h"
#include "GameFramework/PlayerController.h"
#include "CreditsWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	StartPlay->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnStartPlayClicked);
	HowToPlay->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnHowToPlayClicked);
	Credits->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnCreditsClicked);
	ExitGame->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnExitGameClicked);
	ReturnToMenu->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::OnReturnToMenuClicked);

	ReturnToMenu->SetVisibility(ESlateVisibility::Hidden);
	GameGuide->SetVisibility(ESlateVisibility::Hidden);
	CreditsSection->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::OnStartPlayClicked()
{
	PlayAnimation(Transition);
	GetOwningPlayer()->ConsoleCommand(TEXT("open TopDownMap"));
}

void UMainMenuWidget::OnHowToPlayClicked()
{
	PlayAnimation(Transition);
	GameGuide->SetVisibility(ESlateVisibility::Visible);
	ReturnToMenu->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(Transition, /*StartAtTime=*/0.f, /*NumLoopsToPlay=*/1, EUMGSequencePlayMode::Reverse);
}

void UMainMenuWidget::OnCreditsClicked()
{
	PlayAnimation(Transition);
	CreditsSection->SetVisibility(ESlateVisibility::Visible);
	ReturnToMenu->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(Transition, /*StartAtTime=*/0.f, /*NumLoopsToPlay=*/1, EUMGSequencePlayMode::Reverse);
}

void UMainMenuWidget::OnExitGameClicked()
{
	GetOwningPlayer()->ConsoleCommand(TEXT("quit"));
}

void UMainMenuWidget::OnReturnToMenuClicked()
{
	PlayAnimation(Transition);
	GameGuide->SetVisibility(ESlateVisibility::Hidden);
	CreditsSection->SetVisibility(ESlateVisibility::Hidden);

	ReturnToMenu->SetVisibility(ESlateVisibility::Hidden);
	PlayAnimation(Transition, /*StartAtTime=*/0.f, /*NumLoopsToPlay=*/1, EUMGSequencePlayMode::Reverse);
}
