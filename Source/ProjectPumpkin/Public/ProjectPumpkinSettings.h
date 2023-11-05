// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectPumpkinSettings.generated.h"

struct FRuntimeFloatCurve;

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

	UFUNCTION(BlueprintPure, Category = "Project Pumpkin Settings")
	FORCEINLINE float GetTimeToActivate() const { return TimeToActivate; }

	UFUNCTION(BlueprintPure, Category = "Project Pumpkin Settings")
	FORCEINLINE float GetNewZOffset() const { return NewZOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Project Pumpkin Settings")
	FORCEINLINE FRuntimeFloatCurve GetActivationCurve() const {return ActivationCurve; }

private:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Slowing Vines", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f, ClampMax = 1.f, UIMax = 1.f, ToolTip = "Amount of slowing vines that will be \"active\" when the game starts. Zero is no active vines, one means all vines are active."))
	float ActiveVinesRatio;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Slowing Vines", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "How long it will take to activate vines after the activation was requested."))
	float TimeToActivate;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Slowing Vines", meta = (AllowPrivateAccess = true, ClampMin = 0.f, UIMin = 0.f, ToolTip = "How close the height of inactive vine to its original height. Zero means as down as possible."))
	float NewZOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slowing Vines", meta = (AllowPrivateAccess = true, ToolTip = "Defines the manner in which inactive vine will reappear."))
	FRuntimeFloatCurve ActivationCurve;

};
