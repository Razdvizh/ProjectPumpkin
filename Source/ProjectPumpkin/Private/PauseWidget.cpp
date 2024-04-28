// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Animation/WidgetAnimation.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPumpkin/ProjectPumpkinGameMode.h"
#include "ProjectPumpkinHUD.h"
#include "FrameWidget.h"

void UPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReturnToMenu->OnClicked.AddUniqueDynamic(this, &UPauseWidget::OnReturnToMenuClicked);
	ToggleMusic->OnCheckStateChanged.AddUniqueDynamic(this, &UPauseWidget::OnToggleMusic);
}

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(BlurIn);
	if (UFrameWidget* FrameWidget = AProjectPumpkinGameMode::GetPumpkinHUD(GetWorld())->GetMainframeWidget())
	{
		ToggleMusic->SetIsChecked(FrameWidget->IsPlayingMusic());
	}
}

void UPauseWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ReturnToMenu->OnClicked.RemoveDynamic(this, &UPauseWidget::OnReturnToMenuClicked);
	ToggleMusic->OnCheckStateChanged.RemoveDynamic(this, &UPauseWidget::OnToggleMusic);
}

void UPauseWidget::OnReturnToMenuClicked()
{
	GetOwningPlayer()->ConsoleCommand(TEXT("open Menu"));
}

void UPauseWidget::OnToggleMusic(bool bPlayMusic)
{
	if (UFrameWidget* FrameWidget = AProjectPumpkinGameMode::GetPumpkinHUD(GetWorld())->GetMainframeWidget())
	{
		FrameWidget->SetPlayMusic(bPlayMusic);
	}
}
