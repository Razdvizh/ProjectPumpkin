// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class UTextBlock;

/**
 * Widget that holds credits.
 */
UCLASS()
class PROJECTPUMPKIN_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	const FText GetCreditsText() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Credits;

};
