// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPumpkinHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FrameWidget.h"

UFrameWidget* AProjectPumpkinHUD::GetMainframeWidget() const
{
	for (const auto Widget : ActiveWidgets)
	{
		if (Widget->IsA(MainframeWidgetClass))
		{
			return Cast<UFrameWidget>(Widget);
		}
	}

	return nullptr;
}

void AProjectPumpkinHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainframeWidgetClass)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		UFrameWidget* InGameWidget = CreateWidget<UFrameWidget>(PlayerController, MainframeWidgetClass);
		InGameWidget->AddToViewport(INT_MIN);
		ActiveWidgets.Add(InGameWidget);
	}
}
