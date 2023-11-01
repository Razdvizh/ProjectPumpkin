// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class UMassAgentComponent;

/**
 * Helper class for interaction with Mass framework
 */
class MASSHORDE_API UMassHordeHelpers : public UObject
{
public:
	static void DestroyMassAgent(UMassAgentComponent* MassAgent);

private:
	UMassHordeHelpers();
};
