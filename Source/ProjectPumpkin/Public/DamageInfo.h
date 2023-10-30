// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "DamageInfo.generated.h"

USTRUCT(BlueprintType)
struct PROJECTPUMPKIN_API FDamageInfo
{
	GENERATED_USTRUCT_BODY()

	FDamageInfo() = default;

	FDamageInfo(const FDamageEvent& Event, float Damage) : DamageEvent(Event), DamageAmount(Damage) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	FDamageEvent DamageEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Info")
	float DamageAmount;
};
