// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activateable.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActivateable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Represents a "bridge" between concrete activation source and abstract behavior.
 * Make sure to implement all methods in derived classes, otherwise an assertion will be triggered.
 * Aforementioned methods do not have default implementation, so it is not safe to call them from interface.
 */
class PROJECTPUMPKIN_API IActivateable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual void OnActivated(AActor* Activator) PURE_VIRTUAL(&IActivateable::OnActivated, );

	UFUNCTION()
	virtual void OnDeactivated(AActor* Activator) PURE_VIRTUAL(&IActivateable::OnDeactivated, );

};
