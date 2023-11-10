// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectPumpkin.h"
#include "ProjectPumpkinSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"

#define LOCTEXT_NAMESPACE "ProjectPumpkinModule"

DEFINE_LOG_CATEGORY(LogProjectPumpkin)

void FProjectPumpkinModule::StartupModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings"));
	SettingsSection = SettingsModule->RegisterSettings(
		TEXT("Project"),
		TEXT("Game"),
		TEXT("ProjectPumpkinSettings"),
		LOCTEXT("ProjectPumpkinSettingsName", "Project Pumpkin"),
		LOCTEXT("ProjectPumpkinSettingsDescription", "Configure options for Project Pumpkin elements"),
		GetMutableDefault<UProjectPumpkinSettings>());
}

void FProjectPumpkinModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		SettingsModule->UnregisterSettings(TEXT("Project"), TEXT("Game"), TEXT("ProjectPumpkinSettings"));
		SettingsSection.Reset();
	}
}

const UProjectPumpkinSettings* FProjectPumpkinModule::GetProjectPumpkinSettings() const
{
	return StaticCast<UProjectPumpkinSettings*>(SettingsSection->GetSettingsObject().Get());
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_PRIMARY_GAME_MODULE(FProjectPumpkinModule, ProjectPumpkin, "ProjectPumpkin");
