// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Templates/SubclassOf.h"
#include "ActivationVolumeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnVolumeActivatedSignature, UActivationVolumeComponent, OnVolumeActivated, AActor*, Activator);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnVolumeDeactivatedSignature, UActivationVolumeComponent, OnVolumeDeactivated, AActor*, Activator);

class AActor;
struct FHitResult;
struct FPropertyChangedEvent;

/*
* Volume that interpets overlaps as activation signals and delegates them to the owning actor.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTPUMPKIN_API UActivationVolumeComponent : public UBoxComponent
{
	GENERATED_UCLASS_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Activation Volume Component")
	FORCEINLINE TSet<TSubclassOf<AActor>>& GetActivatorClasses() { return ActivatorClasses; }

	UPROPERTY(BlueprintAssignable, Category = "Activation Volume Component")
	FOnVolumeActivatedSignature OnVolumeActivated;

	UPROPERTY(BlueprintAssignable, Category = "Activation Volume Component")
	FOnVolumeDeactivatedSignature OnVolumeDeactivated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnRegister() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activation Volume Component", meta = (OnlyPlaceable, DisplayName = "Activators", ToolTip = "Who can trigger activation events. Doesn't account for derived classes."))
	TSet<TSubclassOf<AActor>> ActivatorClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Activation Volume Component", meta = (ToolTip = "Regulates the ability of classes that are derived from activator classes to act as activators."))
	bool bDerivedAreActivators;

};
