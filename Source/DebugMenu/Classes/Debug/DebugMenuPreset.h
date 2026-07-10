// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "Engine/DataAsset.h"
#include "Debug/DebugMenuConsoleCommandTypes.h"

#include "DebugMenuPreset.generated.h"

class UDebugMenuConsoleCommandBase;

/**
 * Debug Menu Preset - Data Asset for organizing and managing debug menu configurations.
 * 
 * This asset allows developers to create reusable collections of debug menu items
 * that can be easily shared, version controlled, and applied to different levels
 * or scenarios. Presets provide a clean way to organize debug functionality
 * without cluttering Project Settings.
 * 
 * Key Benefits:
 * - Reusable debug configurations across multiple levels
 * - Easy sharing between team members (version control friendly)
 * - Runtime registration/unregistration for dynamic debug menus
 * - Blueprint-friendly workflow for non-programmers
 * 
 * Usage Example:
 * @code
 * // In C++
 * UDebugMenuPreset* PlayerPreset = LoadObject<UDebugMenuPreset>(nullptr, TEXT("/Game/Debug/DA_PlayerDebug"));
 * PlayerPreset->RegisterPreset();
 * 
 * // Or use with DebugMenuActor in level for automatic registration
 * @endcode
 * 
 * Typical Workflow:
 * 1. Create Data Asset in Content Browser (Choose UDebugMenuPreset)
 * 2. Configure debug commands using the visual editor
 * 3. Reference in DebugMenuActor or register manually via Blueprint/C++
 * 4. Commands appear automatically in debug menu when registered
 */
UCLASS(BlueprintType, meta=(DisplayName="Debug Menu Preset"))
class DEBUGMENU_API UDebugMenuPreset : public UDataAsset
{
	GENERATED_BODY()

public:
	UDebugMenuPreset();

public:
	/**
	 * Debug menu commands organized by categories.
	 * 
	 * This map defines all debug menu items for this preset. Each category becomes 
	 * a tab in the debug menu, and the console command definitions determine what 
	 * controls appear within that tab.
	 * 
	 * Each command defines a control type (button, boolean, slider, etc.) and the
	 * console command it should execute. Commands appear in the debug menu in the
	 * order they're listed within each category.
	 * 
	 * Use the dropdown to add different control types:
	 * - Simple Exec: Buttons that execute console commands without parameters
	 * - Boolean: Checkboxes that pass 0/1 to console commands  
	 * - Float: Sliders with min/max bounds
	 * - Integer: Number spinners with bounds
	 * - List: Dropdown menus with predefined options
	 * 
	 * Useful for:
	 * - Setting up collections of related debug commands
	 * - Creating reusable debug configurations
	 * - Organizing debug functionality by game system
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug Menu", meta=(DisplayName="Category Commands"))
	TMap<FName, FDebugMenuCategoryDefinition> CategoryToCommandsMap;
	
public:
	/**
	 * Register all debug commands from this preset with the debug menu system.
	 * Commands will appear in the specified category tab and remain active until
	 * UnregisterPreset() is called or the preset is destroyed.
	 * 
	 * Since DataAssets don't have world context, you must provide the world context
	 * for accessing the DebugMenuSubsystem.
	 * 
	 * Safe to call multiple times - duplicate registrations are ignored.
	 * Safe to call during gameplay - commands appear immediately.
	 * 
	 * @param WorldContext - World context for accessing the DebugMenuSubsystem
	 * @return True if registration succeeded, false if debug menu system unavailable
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", meta=(DisplayName="Register Preset", CallInEditor=true))
	bool RegisterPreset(const UObject* WorldContext);

	/**
	 * Remove all debug commands from this preset from the debug menu system.
	 * The commands will disappear from the debug menu immediately.
	 * 
	 * Since DataAssets don't have world context, you must provide the world context
	 * for accessing the DebugMenuSubsystem.
	 * 
	 * Safe to call multiple times - already unregistered commands are ignored.
	 * Automatically called when the preset is destroyed or garbage collected.
	 * 
	 * @param WorldContext - World context for accessing the DebugMenuSubsystem
	 * @return True if unregistration succeeded, false if nothing to unregister
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", meta=(DisplayName="Unregister Preset", CallInEditor=true))
	bool UnregisterPreset(const UObject* WorldContext);

	/**
	 * Check if this preset is currently registered with the debug menu system.
	 * Useful for conditional logic in Blueprints or debugging registration issues.
	 * 
	 * @param WorldContext - World context for accessing the DebugMenuSubsystem
	 * @return True if preset is registered and commands are visible in debug menu
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Debug Menu", meta=(DisplayName="Is Preset Registered", CallInEditor=true))
	bool IsPresetRegistered(const UObject* WorldContext) const;
};