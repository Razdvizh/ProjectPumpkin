// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Templates/SubclassOf.h"
#include "ProjectPumpkinGameMode.generated.h"

class AProjectPumpkinPlayerController;
class AProjectPumpkinPlayerState;
class AProjectPumpkinGameState;
class AProjectPumpkinCharacter;
class AProjectPumpkinHUD;
class UWorld;
struct FPropertyChangedEvent;

/*
* Project-wide gamemode with type safe access to its classes.
*/
UCLASS(minimalapi)
class AProjectPumpkinGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectPumpkinGameMode();

	PROJECTPUMPKIN_API virtual void PreInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Game Mode", meta = (WorldContext = "WorldContextObject"))
	static PROJECTPUMPKIN_API AProjectPumpkinGameMode* GetPumpkinGameMode(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Player Controller", meta = (WorldContext = "WorldContextObject"))
	static PROJECTPUMPKIN_API AProjectPumpkinPlayerController* GetPumpkinPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Player State", meta = (WorldContext = "WorldContextObject"))
	static PROJECTPUMPKIN_API AProjectPumpkinPlayerState* GetPumpkinPlayerState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Player State", meta = (WorldContext = "WorldContextObject"))
	static PROJECTPUMPKIN_API AProjectPumpkinGameState* GetPumpkinGameState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Character", meta = (WorldContext = "WorldContextObject"))
	static PROJECTPUMPKIN_API AProjectPumpkinCharacter* GetPumpkinCharacter(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "HUD", meta = (WorldContext = "WorldContextObject"))
	static PROJECTPUMPKIN_API AProjectPumpkinHUD* GetPumpkinHUD(const UObject* WorldContextObject);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	static PROJECTPUMPKIN_API UWorld* GetProjectPumpkinWorld(const UObject* WorldContextObject, const FString& FunctionName);

};
