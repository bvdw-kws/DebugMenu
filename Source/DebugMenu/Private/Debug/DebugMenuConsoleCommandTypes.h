// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

#pragma once

#include "CoreMinimal.h"

#include "DebugMenuConsoleCommandTypes.generated.h"

class UImGuiDebugMenu;

/**
 * Base class for defining console command definitions in debug menu settings.
 * 
 * This abstract class provides the foundation for creating different types of
 * debug menu controls that can be configured through Project Settings rather
 * than requiring code changes.
 * 
 * Each derived class represents a specific control type (button, boolean, slider, etc.)
 * and knows how to register itself with the debug menu system during initialization.
 * 
 * The class uses UE's DefaultToInstanced and EditInlineNew meta tags to enable
 * intuitive editing in the Project Settings interface, allowing developers to
 * create and configure debug commands directly in the editor.
 * 
 * Key Responsibilities:
 * - Define common properties for all debug menu commands (name, console command)
 * - Provide registration interface for debug menu integration
 * - Enable Project Settings configuration through UE's property system
 * 
 * Derived classes should override RegisterConsoleCommand() to create appropriate
 * debug menu controls using the debug menu interface.
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class DEBUGMENU_API UDebugMenuConsoleCommandBase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Register this console command definition with the debug menu system.
	 * Called during debug menu initialization to create the appropriate control.
	 * 
	 * @param Category - Debug menu category/tab where this control should appear
	 * @param DebugMenu - Debug menu instance to register the control with
	 */
	virtual void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) {}

protected:
	/**
	 * Display name for this debug menu item.
	 * This text appears as the label next to the control in the debug menu.
	 */
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand, meta=(DisplayName="Display Name"))
	FString CommandName;

	/**
	 * Unreal Engine console command to execute when this control is activated.
	 * Should be a valid UE console command (with or without parameters depending on control type).
	 * 
	 * Examples: "god", "fly", "fov 90", "r.ScreenPercentage"
	 */
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand, meta=(DisplayName="Console Command"))
	FString ConsoleCommand;
};

UCLASS(DisplayName="Simple Exec")
class DEBUGMENU_API UDebugMenuConsoleCommandSimpleExec : public UDebugMenuConsoleCommandBase
{
	GENERATED_BODY()

public:
	void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) override final;
};

UCLASS(DisplayName="Boolean")
class DEBUGMENU_API UDebugMenuConsoleCommandBoolean : public UDebugMenuConsoleCommandBase
{
	GENERATED_BODY()

	UDebugMenuConsoleCommandBoolean();

public:
	void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) override final;

protected:
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	bool DefaultValue;
};

UCLASS(DisplayName = "Integer")
class DEBUGMENU_API UDebugMenuConsoleCommandInt : public UDebugMenuConsoleCommandBase
{
	GENERATED_BODY()

	UDebugMenuConsoleCommandInt();

public:
	void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) override final;

protected:
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	int32 DefaultValue;
	
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	int32 MinValue;
	
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	int32 MaxValue;
	
	/** If true, executes command when loading saved settings */
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand, meta=(DisplayName="Execute On Load"))
	bool bExecOnLoad;
	
	/** If true, automatically executes console command when value changes (like boolean controls) */
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand, meta=(DisplayName="Auto Apply"))
	bool bAutoApply;
};

UCLASS(DisplayName = "Floating Number")
class DEBUGMENU_API UDebugMenuConsoleCommandFloat : public UDebugMenuConsoleCommandBase
{
	GENERATED_BODY()

	UDebugMenuConsoleCommandFloat();

public:
	void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) override final;

protected:
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	float DefaultValue;
	
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	float MinValue;
	
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	float MaxValue;
	
	/** If true, executes command when loading saved settings */
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand, meta=(DisplayName="Execute On Load"))
	bool bExecOnLoad;
	
	/** If true, automatically executes console command when value changes (like boolean controls) */
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand, meta=(DisplayName="Auto Apply"))
	bool bAutoApply;
};

UCLASS(DisplayName = "List")
class DEBUGMENU_API UDebugMenuConsoleCommandList : public UDebugMenuConsoleCommandBase
{
	GENERATED_BODY()

	UDebugMenuConsoleCommandList();

public:
	void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) override final;

protected:
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	TArray<FString> Items;
};

UCLASS(DisplayName = "Input Text")
class DEBUGMENU_API UDebugMenuConsoleCommandInputText : public UDebugMenuConsoleCommandBase
{
	GENERATED_BODY()

	UDebugMenuConsoleCommandInputText();

public:
	void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) override final;
};

/*
* Define each category for our debug menu.
*/
USTRUCT()
struct DEBUGMENU_API FDebugMenuCategoryDefinition
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditDefaultsOnly, Instanced, Category=DebugMenu)
	TArray<UDebugMenuConsoleCommandBase*> ConsoleCommands;
};
