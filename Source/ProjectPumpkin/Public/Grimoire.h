// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivateableActor.h"
#include "Grimoire.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrimoirePickedUpEvent);

class AMassSpawner;
class AFirepit;

UCLASS()
class PROJECTPUMPKIN_API AGrimoire : public AActivateableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrimoire();

	UPROPERTY(BlueprintAssignable, Category = "Grimoire")
	FOnGrimoirePickedUpEvent OnGrimoirePickedUp;

protected:
	virtual void OnActivated(AActor* Activator) final override;

	virtual void OnDeactivated(AActor* Activator) override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "Grimoire")
	AMassSpawner* BossSpawner;

	UPROPERTY(EditInstanceOnly, Category = "Grimoire")
	AFirepit* Firepit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grimoire", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1.f, UIMax = 1.f, ToolTip = "Ratio of how many inactive vines will be left after the grimoire is picked up."))
	float RemainingVinesRatio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grimoire", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "Time it takes to activate vines when the grimoire is picked up. Zero means no vines will be activated."))
	float VineActivationTime;

};
