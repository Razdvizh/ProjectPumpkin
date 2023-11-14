// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MortalCharacter.h"
#include "Interactable.h"
#include "Templates/SubclassOf.h"
#include "ProjectPumpkinCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UHealthComponent;
class UMassAgentComponent;
class AProjectile;
struct FInputActionValue;

UCLASS(Blueprintable)
class AProjectPumpkinCharacter : public AMortalCharacter, public IInteractable
{
	GENERATED_BODY()

public:
	AProjectPumpkinCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void Interact_Implementation(AActor* Initiator) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnDemise() override;

	UFUNCTION()
	virtual void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void Move(const FInputActionValue& Value);

	void Look();

	void Shoot();

private:
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	void DrawCursorHitLocation(const FVector& HitLocation);
#endif

	void OnLookCompleted();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (ToolTip = "Offset for character rotation during look towards the mouse cursor."))
	FRotator LookOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (DisplayName = "Projectile", ToolTip = "Projectile to spawn."))
	TSubclassOf<AProjectile> ProjectileClass;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mass")
	UMassAgentComponent* MassAgent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* ShootAction;

	bool bIsLooking;

};
