// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkinCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#include "ProjectPumpkin.h"
#include "DrawDebugHelpers.h"
#endif

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
static TAutoConsoleVariable<bool> CVarDebugMouseHit(
	TEXT("Pumpkin.DebugMouseHit"),
	false,
	TEXT("Enables visualization of the mouse cursor hit location in the world"),
	ECVF_Default
);
#endif

AProjectPumpkinCharacter::AProjectPumpkinCharacter() : LookOffset(0.f, 180.f, 0.f)
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

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AProjectPumpkinCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
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

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void AProjectPumpkinCharacter::Look()
{
	UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement();
	const bool bIsMoving = CharacterMovementComp->Velocity.Length() > UE_KINDA_SMALL_NUMBER;

	if (Controller && !bIsMoving)
	{
		FHitResult Hit;
		APlayerController* PlayerController = StaticCast<APlayerController*, AController*>(Controller);
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit))
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
