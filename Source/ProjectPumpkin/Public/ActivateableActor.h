// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Activateable.h"
#include "ActivateableActor.generated.h"

class UStaticMeshComponent;
class UActivationVolumeComponent;

/*
* Actor that has active and inactive states which can be altered by Activators.
*/
UCLASS(abstract)
class PROJECTPUMPKIN_API AActivateableActor : public AActor, public IActivateable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AActivateableActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Activateable Actor")
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Activateable Actor")
	FORCEINLINE UActivationVolumeComponent* GetActivationVolume() const { return ActivationVolume; }

protected:
	UFUNCTION()
	virtual void OnActivated(AActor* Activator) override;

	UFUNCTION()
	virtual void OnDeactivated(AActor* Activator) override;

protected:
	static FName MeshName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activateable Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Activateable Actor")
	UActivationVolumeComponent* ActivationVolume;

};
