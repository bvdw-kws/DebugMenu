// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "DebugMenuActor.generated.h"

class UDebugMenuPreset;

/**
 * Debug Menu Actor - Level-placed actor for automatic debug menu registration.
 * 
 * This actor provides a simple drag-and-drop solution for adding debug menu
 * functionality to levels. Place it in your level, assign a debug menu preset,
 * and the preset's commands will automatically appear in the debug menu when
 * the level starts.
 * 
 * Key Benefits:
 * - Level-specific debug functionality without code changes
 * - Automatic lifecycle management (register on BeginPlay, cleanup on EndPlay)
 * - Blueprint-friendly workflow for designers and non-programmers  
 * - Multiple actors can use different presets for organized debug categories
 * - Safe cleanup prevents stale debug menu entries between level transitions
 * 
 * Typical Workflow:
 * 1. Create debug menu preset assets (UDebugMenuPreset)
 * 2. Place DebugMenuActor in your level
 * 3. Assign preset to the actor in the Details panel
 * 4. Play level → debug menu automatically shows preset commands
 * 5. Stop play → commands automatically cleaned up
 * 
 * Advanced Usage:
 * - Use multiple actors with different presets for organized categories
 * - Disable auto-registration and call RegisterDebugMenu() manually
 * - Register/unregister dynamically based on game state via Blueprint
 * - Create child Blueprint classes for project-specific debug behaviors
 * 
 * Thread Safety: All methods should be called from the game thread only.
 */
UCLASS(BlueprintType, Blueprintable, meta=(DisplayName="Debug Menu Actor"))
class DEBUGMENU_API ADebugMenuActor : public AActor
{
	GENERATED_BODY()

public:
	ADebugMenuActor();

protected:
	/**
	 * Debug menu preset asset containing the commands to register.
	 * 
	 * This should reference a UDebugMenuPreset asset created in the Content Browser.
	 * The preset defines which debug menu controls appear and in which category.
	 * Multiple actors can reference the same preset, or each can have unique presets.
	 * 
	 * If null, the actor will log a warning and do nothing during BeginPlay.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug Menu", 
		meta=(DisplayName="Debug Menu Preset"))
	TObjectPtr<UDebugMenuPreset> DebugMenuPreset;

	/**
	 * Whether to automatically register the preset when this actor begins play.
	 * 
	 * When true (default): Preset is registered during BeginPlay() automatically.
	 * When false: You must call RegisterDebugMenu() manually via Blueprint or C++.
	 * 
	 * Disable this if you want conditional registration based on game state,
	 * player permissions, or other runtime conditions.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug Menu", 
		meta=(DisplayName="Auto Register on Begin Play"))
	bool bAutoRegisterOnBeginPlay = true;

	/**
	 * Whether to automatically switch to the preset's category when registered.
	 * 
	 * When true: After successful registration, the debug menu will automatically
	 * switch to display the category specified by this preset. This is useful
	 * for level-specific debug functionality that should be immediately visible.
	 * 
	 * When false (default): The debug menu category is not changed during registration.
	 * 
	 * This only takes effect when the preset is successfully registered, either
	 * automatically during BeginPlay() or manually via RegisterDebugMenu().
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug Menu", 
		meta=(DisplayName="Force Open Preset Category"))
	bool bForceOpenPresetCategory = false;

	// AActor overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	/**
	 * Manually register the debug menu preset with the debug menu system.
	 * 
	 * This makes all commands from the assigned preset appear in the debug menu
	 * under the preset's specified category. Safe to call multiple times - 
	 * duplicate registrations are ignored.
	 * 
	 * Automatically called during BeginPlay() if bAutoRegisterOnBeginPlay is true.
	 * 
	 * @return True if registration succeeded, false if preset is null or registration failed
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", 
		meta=(DisplayName="Register Debug Menu"))
	bool RegisterDebugMenu();

	/**
	 * Manually unregister the debug menu preset from the debug menu system.
	 * 
	 * This removes all commands from the assigned preset from the debug menu.
	 * Safe to call multiple times - already unregistered presets are ignored.
	 * 
	 * Automatically called during EndPlay() to ensure cleanup.
	 * 
	 * @return True if unregistration succeeded, false if nothing was registered
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", 
		meta=(DisplayName="Unregister Debug Menu"))
	bool UnregisterDebugMenu();

	/**
	 * Check if this actor's preset is currently registered with the debug menu.
	 * 
	 * Useful for conditional Blueprint logic or debugging registration state.
	 * Returns false if no preset is assigned.
	 * 
	 * @return True if preset is registered and commands are visible in debug menu
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Debug Menu", 
		meta=(DisplayName="Is Debug Menu Registered"))
	bool IsDebugMenuRegistered() const;

	/**
	 * Get the currently assigned debug menu preset.
	 * Returns null if no preset is assigned.
	 * 
	 * @return The debug menu preset asset, or null if none assigned
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Debug Menu", 
		meta=(DisplayName="Get Debug Menu Preset"))
	UDebugMenuPreset* GetDebugMenuPreset() const { return DebugMenuPreset; }

	/**
	 * Assign a new debug menu preset to this actor.
	 * 
	 * If a preset is already registered, it will be unregistered first before
	 * registering the new preset (if auto-registration is enabled or you call
	 * RegisterDebugMenu() manually).
	 * 
	 * @param NewPreset - The new preset to assign (can be null to clear)
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", 
		meta=(DisplayName="Set Debug Menu Preset"))
	void SetDebugMenuPreset(UDebugMenuPreset* NewPreset);

	/**
	 * Manually switch the debug menu to display this preset's category.
	 * 
	 * This function allows you to programmatically open the debug menu category
	 * associated with this actor's preset. Useful for dynamic category switching
	 * based on gameplay events or user interactions.
	 * 
	 * @return True if the category was successfully switched to, false if no preset or switch failed
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", 
		meta=(DisplayName="Open Preset Category"))
	bool OpenPresetCategory();

private:
	/**
	 * Track registration state to provide accurate status information
	 * and prevent duplicate registration attempts.
	 */
	UPROPERTY(Transient)
	bool bIsCurrentlyRegistered = false;
};