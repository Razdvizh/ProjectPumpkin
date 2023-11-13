// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivateableActor.h"
#include "Firepit.generated.h"

UCLASS()
class PROJECTPUMPKIN_API AFirepit : public AActivateableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirepit();

protected:
	virtual void OnVolumeActivated(AActor* Activator) override;

	virtual void OnVolumeDeactivated(AActor* Activator) override;

};
