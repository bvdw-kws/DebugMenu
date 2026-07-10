// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "Debug/DebugMenuConsoleCommandTypes.h"

#include "DebugMenuSettings.generated.h"

class UDebugMenuPreset;

/**
 * Debug Menu Settings - Configuration class for runtime debug menu behavior.
 * 
 * This settings class provides project-wide configuration for the debug menu system,
 * allowing developers to define default categories, predefined console commands,
 * and other debug menu behaviors through the Project Settings interface.
 * 
 * Settings are stored in the game configuration and can be modified per-project
 * to customize the debug menu experience for different development needs.
 * 
 * Key Configuration Options:
 * - Default category/tab when menu opens
 * - Predefined console command definitions organized by category
 * - Input binding preferences (handled through UE's input system)
 * 
 * Access from Project Settings: Edit -> Project Settings -> Plugins -> Debug Menu
 * 
 * Usage Example:
 * @code
 * const UDebugMenuSettings* Settings = GetDefault<UDebugMenuSettings>();
 * FName DefaultTab = Settings->DefaultCategory;
 * @endcode
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Debug Menu"))
class DEBUGMENU_API UDebugMenuSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UDebugMenuSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	/**
	 * Default category/tab to display when the debug menu is first opened.
	 * If NAME_None or empty, the menu will select the first available category.
	 * This setting helps maintain consistent debug workflow across team members.
	 */
	UPROPERTY(config, EditDefaultsOnly, Category=DebugMenu, meta=(DisplayName="Default Category"))
	FName DefaultCategory = NAME_None;

	/**
	 * Default debug menu preset to load at startup.
	 * 
	 * This preset defines all debug menu categories and commands that should be
	 * available by default. The preset can be easily swapped to change the entire
	 * debug menu configuration without modifying project settings.
	 * 
	 * Benefits of using a preset asset:
	 * - Easy to share debug configurations between team members
	 * - Version control friendly (stored as separate asset file)
	 * - Can be swapped at runtime for different debugging scenarios
	 * - Reusable across multiple projects
	 * 
	 * Leave empty to start with no predefined debug commands (runtime-added commands
	 * via C++ API will still appear).
	 */
	UPROPERTY(config, EditDefaultsOnly, Category=DebugMenu, meta=(DisplayName="Default Debug Preset"))
	TSoftObjectPtr<class UDebugMenuPreset> DefaultDebugPreset;
};
