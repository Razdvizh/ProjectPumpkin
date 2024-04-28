// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivateableActor.h"
#include "Firepit.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTPUMPKIN_API AFirepit final : public AActivateableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirepit();

protected:
	virtual void BeginPlay() override;

	virtual void OnActivated(AActor* Activator) override;

	virtual void OnDeactivated(AActor* Activator) override;

private:
	void MarkAsActivateable();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firepit", meta = (AllowPrivateAccess = true))
	UNiagaraComponent* FireFX;

	bool bCanBeActivated;

	friend class AGrimoire;
};
