// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditsWidget.h"
#include "Components/TextBlock.h"

const FText UCreditsWidget::GetCreditsText() const
{
    checkf(Credits, TEXT("Credits text block is invalid!"))

    return Credits->GetText();
}
