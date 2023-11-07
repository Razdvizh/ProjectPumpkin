// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Templates/SubclassOf.h"
#include "ActivationVolume.generated.h"

class UShapeComponent;
class AActor;

/*
* Volume that interpets overlaps as activation signals and delegates them to the owning actor.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPUMPKIN_API UActivationVolume : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActivationVolume();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
