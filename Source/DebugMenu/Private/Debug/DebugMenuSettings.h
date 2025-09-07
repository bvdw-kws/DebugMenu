// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "DebugMenuConsoleCommandTypes.h"

#include "DebugMenuSettings.generated.h"

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
class UDebugMenuSettings : public UDeveloperSettingsBackedByCVars
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
	UPROPERTY(EditDefaultsOnly, Category=DebugMenu, meta=(DisplayName="Default Category"))
	FName DefaultCategory = NAME_None;

	/**
	 * Predefined console command definitions organized by debug menu categories.
	 * 
	 * This map allows you to configure debug menu items through Project Settings
	 * rather than requiring code changes. Each category becomes a tab in the debug
	 * menu, and the console command definitions determine what controls appear.
	 * 
	 * Useful for:
	 * - Setting up common debug commands across the team
	 * - Configuring project-specific debug utilities
	 * - Creating standardized debug interfaces for different game systems
	 * 
	 * Note: Commands defined here are loaded at startup. Runtime-added commands
	 * (via C++ API) will appear alongside these predefined ones.
	 */
	UPROPERTY(EditDefaultsOnly, Category=DebugMenu, meta=(DisplayName="Category Commands"))
	TMap<FName, FDebugMenuCategoryDefinition> CategoryToCommandsMap;
};
