// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Components/Image.h"
#include "ProjectPumpkin/ProjectPumpkinGameMode.h"
#include "ProjectPumpkin/ProjectPumpkinCharacter.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "HealthComponent.h"
#include "Grimoire.h"
#include "EngineUtils.h"

constexpr float SEG_STEP = 0.2f;

FName UInGameWidget::CurrentHealthParamName(TEXT("CurrentHealth"));

UInGameWidget::UInGameWidget(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer),
	UIHealth(4.f)
{
}

void UInGameWidget::NativeOnInitialized()
{
	Super::OnInitialized();

	if (AProjectPumpkinCharacter* Character = AProjectPumpkinGameMode::GetPumpkinCharacter(GetWorld()))
	{
		UHealthComponent* Health = Character->GetHealthComponent();
		Health->OnHealthChanged.AddUniqueDynamic(this, &UInGameWidget::UpdateHealth);
		Health->OnLethalHealthReached.AddUniqueDynamic(this, &UInGameWidget::UpdateLethalHealth);

		AGrimoire* Grimoire;
		for (TActorIterator<AGrimoire> It(GetWorld()); It; ++It)
		{
			Grimoire = *It;
			Grimoire->OnGrimoirePickedUp.AddUniqueDynamic(this, &UInGameWidget::UpdateGrimoire);
			break;
		}
	}
}

void UInGameWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (AProjectPumpkinCharacter* Character = AProjectPumpkinGameMode::GetPumpkinCharacter(GetWorld()))
	{
		UHealthComponent* Health = Character->GetHealthComponent();
		Health->OnHealthChanged.RemoveDynamic(this, &UInGameWidget::UpdateHealth);
		Health->OnLethalHealthReached.RemoveDynamic(this, &UInGameWidget::UpdateLethalHealth);
	}
}

void UInGameWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (GrimoireImage)
	{
		GrimoireImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameWidget::UpdateHealth(float NewHealth)
{
	if (ensureMsgf(WheelParams, TEXT("Material parameter collection for health UI is invalid!")))
	{
		UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(WheelParams);
		float CurrentHealthParam{ 0.f };
		const bool bIsCurrentHealthParamFound = Instance->GetScalarParameterValue(CurrentHealthParamName, CurrentHealthParam);
		if (ensureMsgf(bIsCurrentHealthParamFound, TEXT("CurrentHealth parameter is not found! Make sure that the name of the parameter is \"%s\""), *CurrentHealthParamName.ToString()))
		{
			const float diff = FMath::FloorToFloat(UIHealth - NewHealth);
			Instance->SetScalarParameterValue(CurrentHealthParamName, CurrentHealthParam + (-SEG_STEP * diff));
			UIHealth = NewHealth;
		}
	}
}

void UInGameWidget::UpdateLethalHealth()
{
	check(WheelParams);
	UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(WheelParams);
	const bool bIsSet = Instance->SetScalarParameterValue(CurrentHealthParamName, 0.f);
	ensureMsgf(bIsSet, TEXT("CurrentHealth parameter is not found! Make sure that the name of the parameter is \"%s\""), *CurrentHealthParamName.ToString());
}

void UInGameWidget::UpdateGrimoire()
{
	GrimoireImage->SetVisibility(ESlateVisibility::Visible);
}
