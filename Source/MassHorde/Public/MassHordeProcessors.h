// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "MassHordeProcessors.generated.h"

UCLASS()
class MASSHORDE_API UChasePlayerProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	UChasePlayerProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
