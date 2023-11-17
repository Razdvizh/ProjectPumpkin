// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

class UMassAgentComponent;
class AMassSpawner;

DECLARE_LOG_CATEGORY_EXTERN(LogHordeHelpers, Display, Verbose);

/**
 * Helper class for interaction with Mass framework
 */
class MASSHORDE_API UMassHordeHelpers : public UObject
{
public:
	static void DestroyMassAgent(UMassAgentComponent* MassAgent);

	static void ResetMassSpawnerConfig(AMassSpawner* Spawner);

private:
	UMassHordeHelpers();

};
