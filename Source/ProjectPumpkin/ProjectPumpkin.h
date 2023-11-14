// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include <type_traits>

class UProjectPumpkinSettings;
class ISettingsSection;

DECLARE_LOG_CATEGORY_EXTERN(LogProjectPumpkin, Log, All);

#define ECC_Mouse ECC_GameTraceChannel1
#define ECC_Projectile ECC_GameTraceChannel2

#define RETURN_ENSURE_NOT_NEGATIVE_OR_NEGATIVE_ZERO(Val, Message) \
    { \
        static_assert(std::is_floating_point<decltype(Val)>::value, "Value must be a floating-point type"); \
        return static_cast<void>(ensureMsgf(!FMath::IsNegativeOrNegativeZero(Val), TEXT("%s"), *Message)); \
    }

class FProjectPumpkinModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	const UProjectPumpkinSettings* GetProjectPumpkinSettings() const;

private:
	TSharedPtr<ISettingsSection> SettingsSection;

};