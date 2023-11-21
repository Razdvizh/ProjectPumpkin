// Copyright Epic Games, Inc. All Rights Reserved.

#pragma region Gameplay
#include "ProjectPumpkinCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectPumpkin/ProjectPumpkinGameMode.h"
#include "ProjectPumpkin/ProjectPumpkinPlayerController.h"
#include "Sound/SoundBase.h"
#include "MassAgentComponent.h"
#include "HealthComponent.h"
#include "DamageInfo.h"
#include "MassHordeHelpers.h"
#include "Interactable.h"
#include "HordeCharacter.h"
#include "HordeBoss.h"
#include "Projectile.h"
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
#pragma region Sound
#include "Audio/ActorSoundParameterInterface.h"
#include "AudioDeviceHandle.h"
#include "AudioDevice.h"
#include "AudioParameter.h"
#pragma endregion Sound
#pragma region Misc
#include "ProjectPumpkin.h"
#include "TimerManager.h"
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

AProjectPumpkinCharacter::AProjectPumpkinCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	  LookOffset(0.f, 180.f, 0.f),
	  RestartLevelDelay(3.f),
	  ProjectileClass(AProjectile::StaticClass()),
	  bIsLooking(false)
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

	Health->SetMaxHealth(4.f);
	Health->SetLethalHealth(0.f);

	MassAgent = CreateDefaultSubobject<UMassAgentComponent>(TEXT("MassAgent"));

	FDamageInfo Info{ FDamageEvent(), 1.f };
	DamageInfoMap.Add(AHordeCharacter::StaticClass(), Info);
	Info.DamageAmount = 2.f;
	DamageInfoMap.Add(AHordeBoss::StaticClass(), Info);

	PrimaryActorTick.bCanEverTick = false;
	CameraBoom->PrimaryComponentTick.bCanEverTick = false;
	TopDownCameraComponent->PrimaryComponentTick.bCanEverTick = false;
	MassAgent->PrimaryComponentTick.bCanEverTick = false;
}

void AProjectPumpkinCharacter::SetProjectileSound(USoundBase* Sound)
{
	ProjectileSound = Sound;
}

void AProjectPumpkinCharacter::SetProjectileClass(TSubclassOf<AProjectile> Class)
{
	ProjectileClass = Class;
}

void AProjectPumpkinCharacter::Interact_Implementation(AActor* Initiator)
{
	const FDamageInfo* DamageInfo = DamageInfoMap.Find(Initiator->GetClass());
	if (DamageInfo)
	{
		TakeDamage((*DamageInfo).DamageAmount, (*DamageInfo).DamageEvent, Initiator->GetInstigatorController(), Initiator);
	}
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

	AActor::OnActorHit.AddUniqueDynamic(this, &AProjectPumpkinCharacter::OnActorHit);
}

void AProjectPumpkinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectPumpkinCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectPumpkinCharacter::Look);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &AProjectPumpkinCharacter::OnLookCompleted);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Canceled, this, &AProjectPumpkinCharacter::OnLookCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AProjectPumpkinCharacter::Shoot);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AProjectPumpkinCharacter::PauseGame);
	}
}

void AProjectPumpkinCharacter::OnDemise()
{
	UWorld* World = GetWorld();
	AProjectPumpkinPlayerController* PlayerController = AProjectPumpkinGameMode::GetPumpkinPlayerController(World);
	
	World->GetTimerManager().SetTimer(RestartLevelHandle, PlayerController, &AProjectPumpkinPlayerController::RestartLevel, RestartLevelDelay);

	Destroy();
}

void AProjectPumpkinCharacter::OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->Implements<UInteractable>())
	{
		IInteractable::Execute_Interact(OtherActor, this);
	}
}

void AProjectPumpkinCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!bIsLooking)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

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

	if (Controller)
	{
		FHitResult Hit;
		APlayerController* PlayerController = StaticCast<APlayerController*, AController*>(Controller);
		if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Mouse, false, Hit))
		{
			bIsLooking = true;
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

			CharacterMovementComp->bOrientRotationToMovement = false;
			CharacterMovementComp->MoveUpdatedComponent(FVector::ZeroVector, NewRotation, false);

			const FRotator NewActorRotation = GetActorRotation() + LookOffset;
			const FRotator YawRotation = FRotator(0.f, NewActorRotation.Yaw, 0.f);
			SetActorRotation(YawRotation);
		}
	}
}

void AProjectPumpkinCharacter::Shoot()
{
	const FTransform HandTransform = GetMesh()->GetBoneTransform(TEXT("hand_r"), RTS_World);
	const FTransform SpawnTransform = FTransform(GetActorRotation(), HandTransform.GetLocation(), HandTransform.GetScale3D());
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	UWorld* World = GetWorld();
	if (GEngine->UseSound() && World->bAllowAudioPlayback && ProjectileSound)
	{
		if (FAudioDeviceHandle AudioDevice = World->GetAudioDevice())
		{
			TArray<FAudioParameter> Params;
			UActorSoundParameterInterface::Fill(this, Params);

			float VolumeMultiplier = 1.f;
			float PitchMultiplier = 1.f;
			float StartTime = 0.01f;
			FVector SoundLocation = SpawnTransform.GetLocation();
			FRotator SoundRotation = SpawnTransform.GetRotation().Rotator();
			AudioDevice->PlaySoundAtLocation(ProjectileSound, World, VolumeMultiplier, PitchMultiplier, StartTime, 
				SoundLocation, SoundRotation, nullptr, nullptr, &Params, this);
		}
	}
	AProjectile* Projectile = Cast<AProjectile>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));
	UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement();
	ProjectileMovement->Velocity += GetCharacterMovement()->Velocity;
}

void AProjectPumpkinCharacter::PauseGame()
{
	AProjectPumpkinPlayerController* PlayerController = AProjectPumpkinGameMode::GetPumpkinPlayerController(GetWorld());
	if (GetWorld()->IsPaused())
	{
		PlayerController->SetPause(false);
		return;
	}

	PlayerController->SetPause(true);
}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
void AProjectPumpkinCharacter::DrawCursorHitLocation(const FVector& HitLocation)
{
	FString DebugMessage = FString::Printf(TEXT("HitLocation: %s"), *HitLocation.ToString());
	GEngine->AddOnScreenDebugMessage(-1, /*TimeToDisplay=*/2.f, FColor::Cyan, DebugMessage);
	DrawDebugPoint(GetWorld(), HitLocation, /*Size=*/3.f, FColor::Red, false, /*LifeTime=*/2.f);
}
#endif

void AProjectPumpkinCharacter::OnLookCompleted()
{
	bIsLooking = false;
}
