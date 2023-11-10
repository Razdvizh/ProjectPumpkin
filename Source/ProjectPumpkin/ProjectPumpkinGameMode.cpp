// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkinGameMode.h"
#include "ProjectPumpkinPlayerController.h"
#include "ProjectPumpkinCharacter.h"
#include "ProjectPumpkinPlayerState.h"
#include "ProjectPumpkinGameState.h"
#include "ProjectPumpkinHUD.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameNetworkManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Engine/World.h"

AProjectPumpkinGameMode::AProjectPumpkinGameMode()
{
	PlayerControllerClass = AProjectPumpkinPlayerController::StaticClass();
	PlayerStateClass = AProjectPumpkinPlayerState::StaticClass();
	GameStateClass = AProjectPumpkinGameState::StaticClass();
	DefaultPawnClass = AProjectPumpkinCharacter::StaticClass();
	HUDClass = AProjectPumpkinPlayerController::StaticClass();
}

void AProjectPumpkinGameMode::PreInitializeComponents()
{
	GetWorldSettings()->GameNetworkManagerClass = nullptr;

	Super::PreInitializeComponents();
}

PROJECTPUMPKIN_API AProjectPumpkinGameMode* AProjectPumpkinGameMode::GetPumpkinGameMode(const UObject* WorldContextObject)
{
	UWorld* World = GetProjectPumpkinWorld(WorldContextObject, TEXT("GetPumpkinPlayerController"));

	return StaticCast<AProjectPumpkinGameMode*>(World->GetAuthGameMode());
}

PROJECTPUMPKIN_API AProjectPumpkinPlayerController* AProjectPumpkinGameMode::GetPumpkinPlayerController(const UObject* WorldContextObject)
{
	UWorld* World = GetProjectPumpkinWorld(WorldContextObject, TEXT("GetPumpkinPlayerController"));

	return StaticCast<AProjectPumpkinPlayerController*>(World->GetFirstPlayerController());
}

PROJECTPUMPKIN_API AProjectPumpkinPlayerState* AProjectPumpkinGameMode::GetPumpkinPlayerState(const UObject* WorldContextObject)
{
	UWorld* World = GetProjectPumpkinWorld(WorldContextObject, TEXT("GetPumpkinPlayerState"));

	return StaticCast<AProjectPumpkinPlayerState*>(World->GetGameState()->PlayerArray[0]);
}

PROJECTPUMPKIN_API AProjectPumpkinGameState* AProjectPumpkinGameMode::GetPumpkinGameState(const UObject* WorldContextObject)
{
	UWorld* World = GetProjectPumpkinWorld(WorldContextObject, TEXT("GetPumpkinGameState"));

	return StaticCast<AProjectPumpkinGameState*>(World->GetGameState());
}

PROJECTPUMPKIN_API AProjectPumpkinCharacter* AProjectPumpkinGameMode::GetPumpkinCharacter(const UObject* WorldContextObject)
{
	UWorld* World = GetProjectPumpkinWorld(WorldContextObject, TEXT("GetPumpkinCharacter"));

	return StaticCast<AProjectPumpkinCharacter*>(GetPumpkinPlayerController(World)->GetPawn());
}

PROJECTPUMPKIN_API AProjectPumpkinHUD* AProjectPumpkinGameMode::GetPumpkinHUD(const UObject* WorldContextObject)
{
	UWorld* World = GetProjectPumpkinWorld(WorldContextObject, TEXT("GetPumpkinHUD"));
	
	return StaticCast<AProjectPumpkinHUD*>(GetPumpkinPlayerController(World)->GetHUD());
}

#if WITH_EDITOR
void AProjectPumpkinGameMode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AProjectPumpkinGameMode, PlayerControllerClass))
	{
		if (!PlayerControllerClass->IsChildOf(APlayerController::StaticClass()))
		{
			PlayerControllerClass = AProjectPumpkinPlayerController::StaticClass();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AProjectPumpkinGameMode, PlayerStateClass))
	{
		if (!PlayerStateClass->IsChildOf(AProjectPumpkinPlayerState::StaticClass()))
		{
			PlayerStateClass = AProjectPumpkinPlayerState::StaticClass();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AProjectPumpkinGameMode, GameStateClass))
	{
		if (!GameStateClass->IsChildOf(AProjectPumpkinGameState::StaticClass()))
		{
			GameStateClass = AProjectPumpkinGameState::StaticClass();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AProjectPumpkinGameMode, DefaultPawnClass))
	{
		if (!DefaultPawnClass->IsChildOf(AProjectPumpkinCharacter::StaticClass()))
		{
			DefaultPawnClass = AProjectPumpkinCharacter::StaticClass();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AProjectPumpkinGameMode, HUDClass))
	{
		if (!HUDClass->IsChildOf(AProjectPumpkinHUD::StaticClass()))
		{
			HUDClass = AProjectPumpkinHUD::StaticClass();
		}
	}
}
#endif

PROJECTPUMPKIN_API UWorld* AProjectPumpkinGameMode::GetProjectPumpkinWorld(const UObject* WorldContextObject, const FString& FunctionName)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

	checkf(World->GetAuthGameMode()->GetClass()->IsChildOf<AProjectPumpkinGameMode>(), TEXT("%s was used in the world that has different Game Mode set."), *FunctionName);

	return World;
}
