// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivateableActor.h"
#include "Grimoire.generated.h"

class AMassSpawner;
class AFirepit;

UCLASS()
class PROJECTPUMPKIN_API AGrimoire : public AActivateableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrimoire();

protected:
	virtual void OnVolumeActivated(AActor* Activator) final override;

	virtual void OnVolumeDeactivated(AActor* Activator) override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "Grimoire")
	AMassSpawner* BossSpawner;

	UPROPERTY(EditInstanceOnly, Category = "Grimoire")
	AFirepit* Firepit;

};
