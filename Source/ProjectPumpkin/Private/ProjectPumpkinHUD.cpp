// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPumpkinHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FrameWidget.h"

void AProjectPumpkinHUD::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	UUserWidget* InGameWidget = CreateWidget<UFrameWidget>(PlayerController, InGameWidgetClass);
	if (InGameWidget)
	{
		InGameWidget->AddToViewport(INT_MAX);
	}
}
