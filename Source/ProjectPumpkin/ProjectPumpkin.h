// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UProjectPumpkinSettings;
class ISettingsSection;

DECLARE_LOG_CATEGORY_EXTERN(LogProjectPumpkin, Log, All);

#define ECC_Mouse ECC_GameTraceChannel1
#define ECC_Projectile ECC_GameTraceChannel2

class FProjectPumpkinModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	const UProjectPumpkinSettings* GetProjectPumpkinSettings() const;

private:
	TSharedPtr<ISettingsSection> SettingsSection;

};