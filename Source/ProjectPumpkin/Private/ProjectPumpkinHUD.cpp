// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPumpkinHUD.h"
#include "ProjectPumpkin/ProjectPumpkinGameMode.h"
#include "ProjectPumpkin/ProjectPumpkinPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.h"

void AProjectPumpkinHUD::BeginPlay()
{
	Super::BeginPlay();

	AProjectPumpkinPlayerController* PlayerController = AProjectPumpkinGameMode::GetPumpkinPlayerController(GetWorld());
	UUserWidget* InGameWidget = CreateWidget<UInGameWidget>(PlayerController, InGameWidgetClass);
	InGameWidget->AddToViewport(INT_MAX);
}
