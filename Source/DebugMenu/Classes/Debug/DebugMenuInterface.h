// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

#pragma once

#include "CoreMinimal.h"

class APlayerController;

/**
 * Handle interface for individual debug menu items.
 * 
 * This interface provides control over debug menu items after they've been created.
 * Currently supports execution operations, but may be extended in the future for
 * dynamic value updates or state queries.
 * 
 * Handles are returned as weak pointers to prevent memory management issues
 * when debug menu items are dynamically removed or the menu is reset.
 */
class DEBUGMENU_API IDebugMenuItemHandle
{
public:
	IDebugMenuItemHandle() = default;
	virtual ~IDebugMenuItemHandle() {}

public:
	/**
	 * Execute this debug menu item programmatically.
	 * Will trigger the same console command that would fire from UI interaction.
	 * @param PlayerController - Controller context for command execution
	 */
	virtual void Exec(APlayerController* PlayerController) = 0;
};

/**
 * Specialized handle interface for list/dropdown debug menu items.
 * 
 * Extends the basic item handle with list-specific functionality for dynamically
 * updating dropdown options and querying current selection. Useful for debug
 * menus that need to reflect changing game state (available levels, active players, etc.).
 */
class DEBUGMENU_API IDebugMenuListHandle
{
public:
	IDebugMenuListHandle() = default;
	virtual ~IDebugMenuListHandle() {}

public:
	/**
	 * Update the available options in this dropdown list.
	 * Useful for dynamic lists that change based on game state.
	 * @param List - New array of string options to display
	 */
	virtual void SetList(const TArray<FString>& List) = 0;
	
	/**
	 * Get the currently selected value from this dropdown list.
	 * @return String value of the currently selected option
	 */
	virtual FString GetValue() = 0;
};

/**
 * Primary interface for creating and managing debug menu items.
 * 
 * This interface provides the main API for adding various types of debug controls
 * to the runtime debug menu system. All controls are organized by category (which
 * appear as tabs in the UI) and execute Unreal Engine console commands when modified.
 * 
 * The debug menu system supports multiple control types:
 * - Boolean toggles (checkboxes)
 * - Numerical sliders and spinners (float/int with bounds)
 * - Text input fields
 * - Dropdown selection lists
 * - Simple execution buttons
 * 
 * All controls integrate with UE's console command system and can optionally
 * save/restore their values across sessions.
 * 
 * Thread Safety: This interface should only be called from the game thread.
 * 
 * Usage Example:
 * @code
 * IDebugMenu& DebugMenu = GetDebugMenuSubsystem()->GetMutableDebugMenu();
 * DebugMenu.AddItem_Bool(TEXT("Player"), TEXT("God Mode"), false, TEXT("god"));
 * DebugMenu.AddItem_Float(TEXT("Graphics"), TEXT("FOV"), 90.0f, 60.0f, 120.0f, TEXT("fov"));
 * @endcode
 */
class DEBUGMENU_API IDebugMenu
{
public:
	IDebugMenu() = default;
	virtual ~IDebugMenu() {}

public:
	/**
	 * Add a simple button that executes a console command when clicked.
	 * Perfect for commands that don't require parameters (like "god", "fly", "quit").
	 * 
	 * @param Category - Tab name where this control appears (creates tab if new)
	 * @param Label - Display text for the button
	 * @param ConsoleCommand - UE console command to execute (without parameters)
	 * @return Handle for programmatic control, or invalid if creation failed
	 */
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_SimpleExec(const FName& Category, FString Label, FString ConsoleCommand) = 0;
	
	/**
	 * Add a text input field that passes entered text to a console command.
	 * Useful for commands that accept string parameters (like "open MapName").
	 * 
	 * @param Category - Tab name where this control appears
	 * @param Label - Display text next to the input field  
	 * @param ConsoleCommand - Console command that receives the text as parameter
	 * @return Handle for programmatic control, or invalid if creation failed
	 */
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_InputText(const FName& Category, FString Label, FString ConsoleCommand) = 0;
	
	/**
	 * Add a boolean checkbox that executes a console command with 0/1 parameter.
	 * Perfect for toggle-style console variables and commands.
	 * 
	 * @param Category - Tab name where this control appears
	 * @param Label - Display text next to the checkbox
	 * @param bDefaultValue - Initial checkbox state  
	 * @param ConsoleCommand - Console command that receives 0 (unchecked) or 1 (checked)
	 * @param bExecOnLoad - If true, executes command when loading saved settings
	 * @return Handle for programmatic control, or invalid if creation failed
	 */
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_Bool(const FName& Category, FString Label, bool bDefaultValue, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
	
	/**
	 * Add a float slider with minimum and maximum bounds.
	 * Provides smooth adjustment of numerical console variables and parameters.
	 * 
	 * @param Category - Tab name where this control appears
	 * @param Label - Display text next to the slider
	 * @param DefaultValue - Initial slider value
	 * @param MinValue - Minimum allowed value (left side of slider)
	 * @param MaxValue - Maximum allowed value (right side of slider)  
	 * @param ConsoleCommand - Console command that receives the float value
	 * @param bExecOnLoad - If true, executes command when loading saved settings
	 * @return Handle for programmatic control, or invalid if creation failed
	 */
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_Float(const FName& Category, FString Label, float DefaultValue, float MinValue, float MaxValue, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
	
	/**
	 * Add an integer spinner with minimum and maximum bounds.
	 * Perfect for discrete numerical settings that don't require fractional values.
	 * 
	 * @param Category - Tab name where this control appears
	 * @param Label - Display text next to the spinner
	 * @param DefaultValue - Initial spinner value
	 * @param MinValue - Minimum allowed value
	 * @param MaxValue - Maximum allowed value
	 * @param ConsoleCommand - Console command that receives the integer value
	 * @param bExecOnLoad - If true, executes command when loading saved settings  
	 * @return Handle for programmatic control, or invalid if creation failed
	 */
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_Int(const FName& Category, FString Label, int32 DefaultValue, int32 MinValue, int32 MaxValue, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
	
	/**
	 * Add a dropdown list with predefined string options.
	 * Excellent for commands that accept specific string parameters (view modes, levels, etc.).
	 * 
	 * @param Category - Tab name where this control appears
	 * @param Label - Display text next to the dropdown
	 * @param ListItems - Array of available options to display in dropdown
	 * @param ConsoleCommand - Console command that receives selected string as parameter
	 * @param bExecOnLoad - If true, executes command when loading saved settings
	 * @return Handle for programmatic control and list updates, or invalid if creation failed
	 */
	virtual TWeakPtr<IDebugMenuListHandle> AddItem_List(const FName& Category, FString Label, const TArray<FString>& ListItems, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
};
