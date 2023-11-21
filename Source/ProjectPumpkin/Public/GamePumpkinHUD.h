// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectPumpkinHUD.h"
#include "Templates/SubclassOf.h"
#include "GamePumpkinHUD.generated.h"

class UUserWidget;

/**
 * Gameplay heads-up display.
 */
UCLASS()
class PROJECTPUMPKIN_API AGamePumpkinHUD : public AProjectPumpkinHUD
{
	GENERATED_BODY()

public:
	AGamePumpkinHUD();

protected:
	virtual void BeginPlay() override;

	//Create and add to viewport, or remove from it if widget is already displayed.
	UFUNCTION(BlueprintCallable, Category = "UI")
	void TogglePauseWidget();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	int32 PauseWidgetZOrder;

};
