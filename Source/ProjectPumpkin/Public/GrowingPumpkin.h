// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrowingPumpkin.generated.h"

class UStaticMeshComponent;
class UActivationVolumeComponent;
class AActor;
class UCurveFloat;
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
class PROJECTPUMPKIN_API AGrowingPumpkin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrowingPumpkin();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Growing Pumpkin")
	FOnGrowingStageReachedSignature OnGrowingStageReached;
	
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Growing Pumpkin")
	FORCEINLINE UStaticMeshComponent* GetPumpkinMesh() const {return PumpkinMesh; }

	UFUNCTION(BlueprintGetter, Category = "Growing Pumpkin")
	FORCEINLINE float GetGrowingTime() const { return GrowingTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Growing Pumpkin")
	FORCEINLINE UCurveFloat* GetGrowingCurve() const { return GrowingCurve; }

	UFUNCTION(BlueprintGetter, Category = "Growing Pumpkin")
	FORCEINLINE float GetPauseBetweenStages() const { return PauseBetweenStages; }

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void SetGrowingCurve(UCurveFloat* Curve);

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void SetGrowingTime(float Time);

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void SetPauseBetweenStages(float Time);

	UFUNCTION(BlueprintCallable, Category = "Growing Pumpkin")
	void SetSizeForStage(EGrowingStage InStage, const FVector& InSize);

	FORCEINLINE const TMap<EGrowingStage, FVector> GetStageSizesMap() const { return StageSizes; }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStageReached(EGrowingStage GrowingStage);

	UFUNCTION()
	virtual void OnPumpkinActivated(AActor* Activator);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Growing Pumpkin", meta = (DisplayName = "Pumpkin", ToolTip = "Pumpkin mesh to grow. Mesh scale on begin play must be equal to the scale specified in StageSizes."))
	UStaticMeshComponent* PumpkinMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Growing Pumpkin")
	UActivationVolumeComponent* ActivationVolume;

	UPROPERTY(EditAnywhere, BlueprintSetter = SetGrowingCurve, Category = "Growing Pumpkin", meta = (ToolTip = "Growing rate ratio. Must have three keys for small, medium and large stages."))
	UCurveFloat* GrowingCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, Category = "Growing Pumpkin", meta = (ToolTip = "Pumpkin size (scale) for each growing stage."))
	TMap<EGrowingStage, FVector> StageSizes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetGrowingTime, Category = "Growing Pumpkin", meta = (CampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "Time that pumpkin takes to fully grow. Zero means that pumpkin will spawn fully grown."))
	float GrowingTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetPauseBetweenStages, Category = "Growing Pumpkin", meta = (ClampMin = 0.f, UIMin = 0.f, Units = "s", ToolTip = "How long the pumpkin will stay at the stage before continuing growing. Zero means no pause."))
	float PauseBetweenStages;

private:
	EGrowingStage Stage;

	float CurrentGrowingTime;

	FVector InitialScale3D;

	FTimerHandle StagePauseHandle;

};
