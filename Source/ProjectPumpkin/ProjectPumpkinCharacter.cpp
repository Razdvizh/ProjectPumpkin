// Copyright Epic Games, Inc. All Rights Reserved.

#pragma region Gameplay
#include "ProjectPumpkinCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "MassAgentComponent.h"
#include "HealthComponent.h"
#include "DamageInfo.h"
#pragma endregion Gameplay
#pragma region Input
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#pragma endregion Input
#pragma region Engine
#include "Engine/SkeletalMesh.h"
#include "Engine/HitResult.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#pragma endregion Engine
#pragma region Misc
#include "ProjectPumpkin.h"
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#include "DrawDebugHelpers.h"
#endif
#pragma endregion Misc

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
static TAutoConsoleVariable<bool> CVarDebugMouseHit(
	TEXT("Pumpkin.DebugMouseHit"),
	false,
	TEXT("Enables visualization of the mouse cursor hit location in the world"),
	ECVF_Default
);
#endif

AProjectPumpkinCharacter::AProjectPumpkinCharacter() 
	: LookOffset(0.f, 180.f, 0.f),
	  DamageInfo(FDamageEvent(), 0.f)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->SetMaxHealth(4.f);
	Health->SetLethalHealth(0.f);

	MassAgent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgent"));

	PrimaryActorTick.bCanEverTick = false;
	CameraBoom->PrimaryComponentTick.bCanEverTick = false;
	TopDownCameraComponent->PrimaryComponentTick.bCanEverTick = false;
	Health->PrimaryComponentTick.bCanEverTick = false;
	MassAgent->PrimaryComponentTick.bCanEverTick = false;
}

float AProjectPumpkinCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const EHealthAssignmentResult Result = Health->Damage(DamageAmount);

	if (Result == EHealthAssignmentResult::Ok)
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	return 0.f;
}

void AProjectPumpkinCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		PlayerController->SetControlRotation(CameraBoom->GetComponentRotation());
	}
}

void AProjectPumpkinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectPumpkinCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectPumpkinCharacter::Look);
	}
}

void AProjectPumpkinCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AProjectPumpkinCharacter::Look()
{
	UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();
	const bool bIsMoving = CharacterMovementComp->Velocity.Length() > UE_KINDA_SMALL_NUMBER;

	if (Controller && !bIsMoving)
	{
		FHitResult Hit;
		APlayerController* PlayerController = StaticCast<APlayerController*, AController*>(Controller);
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Floor, false, Hit))
		{
			const FVector MousePosition = FVector(Hit.ImpactPoint.X, Hit.ImpactPoint.Y, 0.f);
			#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			if (CVarDebugMouseHit.GetValueOnGameThread())
			{
				DrawCursorHitLocation(MousePosition);
			}
			#endif
			const FQuat TargetRotation = ((CharacterMovementComp->GetLastUpdateLocation()) - MousePosition).ToOrientationQuat();
			const FQuat DeltaRotation = CharacterMovementComp->GetDeltaRotation(GetWorld()->DeltaTimeSeconds).Quaternion();
			const FQuat NewRotation = TargetRotation * DeltaRotation;

			CharacterMovementComp->MoveUpdatedComponent(FVector::ZeroVector, NewRotation, false);

			const FRotator NewActorRotation = GetActorRotation() + LookOffset;
			SetActorRotation(NewActorRotation);
		}
	}
}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
void AProjectPumpkinCharacter::DrawCursorHitLocation(const FVector& HitLocation)
{
	FString DebugMessage = FString::Printf(TEXT("HitLocation: %s"), *HitLocation.ToString());
	GEngine->AddOnScreenDebugMessage(-1, /*TimeToDisplay=*/2.f, FColor::Cyan, DebugMessage);
	DrawDebugPoint(GetWorld(), HitLocation, /*Size=*/3.f, FColor::Red, false, /*LifeTime=*/2.f);
}
#endif
