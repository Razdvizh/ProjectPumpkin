// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DamageInfo.h"
#include "ProjectPumpkinCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UHealthComponent;
class UMassAgentComponent;
struct FInputActionValue;

UCLASS(Blueprintable)
class AProjectPumpkinCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectPumpkinCharacter();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	void Look();

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
private:
	void DrawCursorHitLocation(const FVector& HitLocation);
#endif

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (ToolTip = "Offset for character rotation during look towards the mouse cursor"))
	FRotator LookOffset;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	UHealthComponent* Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mass")
	UMassAgentComponent* MassAgent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	FDamageInfo DamageInfo;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = true))
	UInputAction* LookAction;
};

