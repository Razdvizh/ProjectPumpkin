// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePumpkinHUD.h"
#include "Blueprint/UserWidget.h"
#include "GamePumpkinPlayerController.h"
#include "FrameWidget.h"

AGamePumpkinHUD::AGamePumpkinHUD()
	: PauseWidgetZOrder(INT_MIN + 1)
{
}

void AGamePumpkinHUD::BeginPlay()
{
	Super::BeginPlay();

	AGamePumpkinPlayerController* PlayerController = StaticCast<AGamePumpkinPlayerController*>(GetOwningPlayerController());

	PlayerController->OnGamePaused.AddUniqueDynamic(this, &AGamePumpkinHUD::TogglePauseWidget);
	PlayerController->OnGameUnPaused.AddUniqueDynamic(this, &AGamePumpkinHUD::TogglePauseWidget);
}

void AGamePumpkinHUD::TogglePauseWidget()
{
	UFrameWidget* FrameWidget = GetMainframeWidget();

	for (const auto Widget : ActiveWidgets)
	{
		if (Widget->IsA(PauseWidgetClass))
		{
			Widget->RemoveFromParent();
			if (FrameWidget)
			{
				FrameWidget->SetVisibility(ESlateVisibility::Visible);
			}
			ActiveWidgets.Remove(Widget);
			Widget->Destruct();
			return;
		}
	}

	if (FrameWidget)
	{
		FrameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	UUserWidget* PauseWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), PauseWidgetClass);
	PauseWidget->AddToViewport(PauseWidgetZOrder);
	ActiveWidgets.Add(PauseWidget);
}
