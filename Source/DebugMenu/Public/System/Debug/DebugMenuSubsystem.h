// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "Engine/World.h"

#include "DebugMenuSubsystem.generated.h"

class IDebugMenu;
class APlayerController;

/**
 * Multicast delegate fired when debug menu is toggled open/closed.
 * @param bOpen - True if menu was opened, false if closed
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FDebugMenuToggleSignature, bool /*bOpen*/);

#ifndef WITH_DEBUG_MENU
#define WITH_DEBUG_MENU 0
#endif // WITH_DEBUG_MENU

/**
 * Debug Menu Subsystem - Main entry point for runtime debug menu functionality.
 * 
 * This subsystem manages the lifecycle of the debug menu system, handles input processing,
 * and provides the primary interface for accessing debug menu functionality. It automatically
 * handles ImGui integration, input routing, and menu state persistence.
 * 
 * The subsystem is automatically excluded from shipping builds and only runs in development
 * configurations. It provides a global access point for adding debug controls and managing
 * debug menu state across the entire game session.
 * 
 * Key Features:
 * - Global debug menu management and access
 * - Input handling (keyboard and gamepad)
 * - Menu state persistence (save/load)
 * - Multi-PIE support with per-instance contexts
 * - Automatic exclusion from shipping builds
 * 
 * Usage:
 * @code
 * UDebugMenuSubsystem* DebugSubsystem = GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>();
 * IDebugMenu& DebugMenu = DebugSubsystem->GetMutableDebugMenu();
 * DebugMenu.AddItem_Bool(TEXT("Player"), TEXT("God Mode"), false, TEXT("god"));
 * @endcode
 */
UCLASS()
class DEBUGMENU_API UDebugMenuSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UDebugMenuSubsystem();

	/**
	 * Check if the debug menu is currently open and visible.
	 * @return True if menu is open and accepting input
	 */
	bool IsMenuOpened() const;
	
	/**
	 * Reset the debug menu window to its default size and position.
	 * Useful for fixing display issues or resetting after resolution changes.
	 */
	void ResetDebugWindow();

	/**
	 * Serialize current debug menu state to a byte array for persistence.
	 * Includes all control values, window layout, and category states.
	 * @param OutMemory - Output array to store serialized data
	 */
	void SaveDebugMenu(TArray<uint8>& OutMemory) const;
	
	/**
	 * Restore debug menu state from previously saved byte array.
	 * Will restore control values, window layout, and trigger ExecOnLoad commands.
	 * @param Memory - Previously saved debug menu data
	 */
	void LoadDebugMenu(const TArray<uint8>& Memory);

	/**
	 * Get the debug menu interface for adding controls and managing categories.
	 * This is the primary way to interact with the debug menu system.
	 * @return Reference to the debug menu interface
	 */
	IDebugMenu& GetMutableDebugMenu();
	
	/**
	 * Register all debug commands from a preset with the debug menu system.
	 * The subsystem will track the registered handles for proper cleanup.
	 * 
	 * @param Preset - Debug menu preset containing commands to register
	 * @return True if registration succeeded, false if preset is null or registration failed
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", meta=(DisplayName="Register Preset"))
	bool RegisterPreset(class UDebugMenuPreset* Preset);
	
	/**
	 * Unregister all debug commands from a previously registered preset.
	 * Removes all commands that were registered with RegisterPreset().
	 * 
	 * @param Preset - Debug menu preset containing commands to unregister  
	 * @return True if unregistration succeeded, false if preset was not registered
	 */
	UFUNCTION(BlueprintCallable, Category="Debug Menu", meta=(DisplayName="Unregister Preset"))
	bool UnregisterPreset(class UDebugMenuPreset* Preset);
	
	/**
	 * Check if a preset is currently registered with the debug menu system.
	 * 
	 * @param Preset - Preset to check registration status for
	 * @return True if preset is registered and commands are visible in debug menu
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Debug Menu", meta=(DisplayName="Is Preset Registered"))
	bool IsPresetRegistered(class UDebugMenuPreset* Preset) const;
	
	/**
	 * Delegate broadcast when debug menu is toggled open or closed.
	 * Subscribe to this to react to menu state changes in your game code.
	 */
	FDebugMenuToggleSignature OnToggleDebugMenu;

protected:
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	// FTickableGameObject implementation Begin
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// FTickableGameObject implementation End
	
private:
	/** The ImGui-based debug menu implementation instance */
	UPROPERTY(Transient)
	TObjectPtr<class UImGuiDebugMenu> ImGuiDebugMenu;
	
	/** Input state tracking for debug menu toggle key */
	UPROPERTY(Transient)
	bool bIsPressDebugMenuTriggerKey{ false };
	
	/** Gamepad input state - left thumbstick held down */
	UPROPERTY(Transient)
	bool bIsHoldGamepadLeftThumbstick{ false };
	
	/** Gamepad input state - right thumbstick held down */
	UPROPERTY(Transient)
	bool bIsHoldGamepadRightThumbstick{ false };
	
	/** Input state for menu visibility toggle key */
	UPROPERTY(Transient)
	bool bIsPressDebugMenuShowKey{ false };

	/**
	 * Check if debug menu should be toggled based on current input state.
	 * Handles both keyboard and gamepad input combinations.
	 * @param PlayerController - Controller to check input from
	 * @return True if menu should be toggled
	 */
	bool CheckToggleDebugMenu(APlayerController* PlayerController);
	
	/**
	 * Check if debug menu page/category should be changed based on input.
	 * Allows navigation between different debug menu tabs using input.
	 * @param PlayerController - Controller to check input from  
	 * @return True if page was changed
	 */
	bool CheckChangePage(APlayerController* PlayerController);

	/**
	 * Map of registered presets to their debug menu item handles.
	 * This allows the subsystem to properly clean up preset registrations.
	 */
	TMap<TWeakObjectPtr<class UDebugMenuPreset>, TArray<TWeakPtr<class IDebugMenuItemHandle>>> RegisteredPresets;

};
