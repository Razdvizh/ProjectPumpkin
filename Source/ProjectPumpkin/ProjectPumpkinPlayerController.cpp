// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkinPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectPumpkinCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

AProjectPumpkinPlayerController::AProjectPumpkinPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

//void AProjectPumpkinPlayerController::BeginPlay()
//{
//	// Call the base class  
//	Super::BeginPlay();
//
//	//Add Input Mapping Context
//	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
//	{
//		Subsystem->AddMappingContext(DefaultMappingContext, 0);
//	}
//}
//
//void AProjectPumpkinPlayerController::SetupInputComponent()
//{
//	// set up gameplay key bindings
//	Super::SetupInputComponent();
//
//	// Set up action bindings
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
//	{
//		// Setup mouse input events
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AProjectPumpkinPlayerController::OnInputStarted);
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AProjectPumpkinPlayerController::OnSetDestinationTriggered);
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AProjectPumpkinPlayerController::OnSetDestinationReleased);
//		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AProjectPumpkinPlayerController::OnSetDestinationReleased);
//
//		// Setup touch input events
//		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AProjectPumpkinPlayerController::OnInputStarted);
//		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AProjectPumpkinPlayerController::OnTouchTriggered);
//		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AProjectPumpkinPlayerController::OnTouchReleased);
//		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AProjectPumpkinPlayerController::OnTouchReleased);
//	}
//	else
//	{
//		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
//	}
//}
//
//void AProjectPumpkinPlayerController::OnInputStarted()
//{
//	StopMovement();
//}
//
//// Triggered every frame when the input is held down
//void AProjectPumpkinPlayerController::OnSetDestinationTriggered()
//{
//	// We flag that the input is being pressed
//	FollowTime += GetWorld()->GetDeltaSeconds();
//	
//	// We look for the location in the world where the player has pressed the input
//	FHitResult Hit;
//	bool bHitSuccessful = false;
//	if (bIsTouch)
//	{
//		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
//	}
//	else
//	{
//		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
//	}
//
//	// If we hit a surface, cache the location
//	if (bHitSuccessful)
//	{
//		CachedDestination = Hit.Location;
//	}
//	
//	// Move towards mouse pointer or touch
//	APawn* ControlledPawn = GetPawn();
//	if (ControlledPawn != nullptr)
//	{
//		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
//		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
//	}
//}
//
//void AProjectPumpkinPlayerController::OnSetDestinationReleased()
//{
//	// If it was a short press
//	if (FollowTime <= ShortPressThreshold)
//	{
//		// We move there and spawn some particles
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
//		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
//	}
//
//	FollowTime = 0.f;
//}
//
//// Triggered every frame when the input is held down
//void AProjectPumpkinPlayerController::OnTouchTriggered()
//{
//	bIsTouch = true;
//	OnSetDestinationTriggered();
//}
//
//void AProjectPumpkinPlayerController::OnTouchReleased()
//{
//	bIsTouch = false;
//	OnSetDestinationReleased();
//}
