// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivateableActor.h"
#include "GrowingPumpkin.generated.h"

class UStaticMeshComponent;
class UActivationVolumeComponent;
class UCurveVector;
struct FPropertyChangedEvent;
struct FTimerHande;

UENUM(BlueprintType)
enum class EGrowingStage : uint8
{
	None = 0 UMETA(Hidden), //Never growing, here only to indicate undefined behavior
	Small = 1, //Small pumpkin, all growing pumpkins spawn small
	Medium = 2, //Medium pumpkin
	Large = 3, //Large pumpkin, final growing stage
};
ENUM_RANGE_BY_FIRST_AND_LAST(EGrowingStage, EGrowingStage::Small, EGrowingStage::Large);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnGrowingStageReachedSignature, AGrowingPumpkin, OnGrowingStageReached, EGrowingStage, StageReached);

UCLASS()
class PROJECTPUMPKIN_API AGrowingPumpkin : public AActivateableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrowingPumpkin();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Growing Pumpkin")
	FOnGrowingStageReachedSignature OnGrowingStageReached;

	UFUNCTION(BlueprintGetter, Category = "Growing Pumpkin")
	FORCEINLINE float GetGrowingTime() const { return GrowingTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Growing Pumpkin")
	FORCEINLINE UCurveVector* GetGrowingCurve() const { return GrowingCurve; }

	UFUNCTION(BlueprintGetter, Category = "Growing Pumpkin")
	FORCEINLINE float GetPauseBetweenStages() const { return PauseBetweenStages; }

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void SetGrowingCurve(UCurveVector* Curve);

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void SetGrowingTime(float Time);

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void SetPauseBetweenStages(float Time);

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void StartGrowth();

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin", meta = (ToolTip = "Reset pumpkin back to growing stage."))
	void ResetGrowth();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStageReached(EGrowingStage GrowingStage);

	virtual void OnVolumeActivated(AActor* Activator);

	virtual void OnVolumeDeactivated(AActor* Activator);

protected:
	UPROPERTY(EditAnywhere, BlueprintSetter = SetGrowingCurve, NoClear, Category = "Growing Pumpkin", meta = (ToolTip = "Growing rate ratio (First key time = 0, Last key time = 1). Must have three keys for small, medium and large stages."))
	UCurveVector* GrowingCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, Category = "Growing Pumpkin", meta = (ValidEnumValues = "Medium, Large", ToolTip = "Amount of health the character will receive during overlapping with pumpkin that reached corresponding stage."))
	TMap<EGrowingStage, float> StageHeal;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetGrowingTime, BlueprintSetter = SetGrowingTime, Category = "Growing Pumpkin", meta = (CampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "Time that pumpkin takes to fully grow. Zero means that pumpkin will spawn fully grown."))
	float GrowingTime;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetPauseBetweenStages, BlueprintSetter = SetPauseBetweenStages, Category = "Growing Pumpkin", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "How long the pumpkin will stay at the stage before continuing growing. Zero means no pause."))
	float PauseBetweenStages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Growing Pumpkin", meta = (ToolTip = "Multiplier that is applied to actor Z location when growing."))
	float MeshLocationMultiplier;

private:
	UPROPERTY(VisibleAnywhere, Category = "Growing Pumpkin")
	EGrowingStage Stage;

	float CurrentGrowingTime;

	bool bNeedsToTick;

	FVector InitialCurveScale;

	FVector InitialLocation;

	FTimerHandle StagePauseHandle;

};
