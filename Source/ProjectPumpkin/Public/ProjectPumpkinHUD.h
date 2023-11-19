// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Templates/SubclassOf.h"
#include "ProjectPumpkinHUD.generated.h"

class UUserWidget;

/**
 * Strongly typed heads-up display for project-wide gamemode
 */
UCLASS()
class PROJECTPUMPKIN_API AProjectPumpkinHUD final : public AHUD
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InGameWidgetClass;

};
