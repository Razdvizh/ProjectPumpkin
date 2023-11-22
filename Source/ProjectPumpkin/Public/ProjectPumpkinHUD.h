// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Templates/SubclassOf.h"
#include "ProjectPumpkinHUD.generated.h"

class UFrameWidget;

/**
 * Strongly typed heads-up display for project-wide gamemode.
 */
UCLASS()
class PROJECTPUMPKIN_API AProjectPumpkinHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFrameWidget* GetMainframeWidget() const;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UFrameWidget> MainframeWidgetClass;

	TSet<UUserWidget*> ActiveWidgets;

};
