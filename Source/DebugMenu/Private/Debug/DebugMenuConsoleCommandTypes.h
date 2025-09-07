// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "CoreMinimal.h"

#include "DebugMenuConsoleCommandTypes.generated.h"

class UImGuiDebugMenu;

/*
* Base class to define a console command for our debug menu
*/
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class DEBUGMENU_API UDebugMenuConsoleCommandBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu) {}

protected:
	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
	FString CommandName;

	UPROPERTY(EditDefaultsOnly, Category=ConsoleCommand)
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
