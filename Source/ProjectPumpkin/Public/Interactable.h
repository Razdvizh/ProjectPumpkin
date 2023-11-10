// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "Interactable.generated.h"

class AActor;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for class interactions that allows for specific actions based on interaction causer.
 */
class PROJECTPUMPKIN_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	* Action that should occur on interaction. Default implementation calls unique interactions.
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Interactable")
	void Interact(AActor* Initiator);

};
