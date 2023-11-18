// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UImage;
class UMaterialParameterCollection;

/**
 * Widget that updates and visualizes health and grimoire status.
 */
UCLASS()
class PROJECTPUMPKIN_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInGameWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnInitialized() override;

	virtual void SynchronizeProperties() override;

	UFUNCTION()
	void UpdateHealth(float NewHealth);

	UFUNCTION()
	void UpdateLethalHealth();
	
	UFUNCTION()
	void UpdateGrimoire();

protected:
	static FName CurrentHealthParamName;

	UPROPERTY(meta = (BindWidget))
	UImage* HealthWheel;

	UPROPERTY(meta = (BindWidget))
	UImage* GrimoireImage;

	float UIHealth;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Widget", meta = (AllowPrivateAccess = true))
	UMaterialParameterCollection* WheelParams;

};
