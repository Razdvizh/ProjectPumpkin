// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectPumpkinSettings.generated.h"

/**
 * Settings for project pumpkin project. Can be accessed through CDO or getter in primary game module.
 */
UCLASS(config = "Game", defaultconfig, meta = (DisplayName = "Project Pumpkin"))
class PROJECTPUMPKIN_API UProjectPumpkinSettings final : public UObject
{
	GENERATED_BODY()
	
public:
	UProjectPumpkinSettings();

	UFUNCTION(BlueprintPure, Category = "Project Pumpkin Settings")
	FORCEINLINE float GetActiveVinesRatio() const { return ActiveVinesRatio; }

private:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Slowing Vines", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f, ClampMax = 1.f, UIMax = 1.f, ToolTip = "Amount of slowing vines that will be \"active\" when the game starts. Zero is no active vines, one means all vines are active."))
	float ActiveVinesRatio;

};
