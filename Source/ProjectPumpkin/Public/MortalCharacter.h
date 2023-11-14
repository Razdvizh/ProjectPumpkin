// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageInfo.h"
#include "MortalCharacter.generated.h"

class UHealthComponent;
struct FDamageEvent;

UCLASS()
class PROJECTPUMPKIN_API AMortalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMortalCharacter(const FObjectInitializer& ObjectInitialier = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Mortal Character")
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Mortal Character")
	FORCEINLINE FDamageInfo GetDamageInfo() const { return DamageInfo; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UFUNCTION()
	virtual void OnDemise() PURE_VIRTUAL(&AMortalCharacter::OnDemise, );

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	FDamageInfo DamageInfo;

};
