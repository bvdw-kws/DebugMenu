// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "Debug/DebugMenuConsoleCommandTypes.h"

#include "Debug/ImGui/ImGuiDebugMenu.h"

void UDebugMenuConsoleCommandSimpleExec::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_SimpleExec(Category, CommandName, ConsoleCommand);
}

UDebugMenuConsoleCommandBoolean::UDebugMenuConsoleCommandBoolean()
	: Super()
{
	DefaultValue = false;
}

void UDebugMenuConsoleCommandBoolean::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_Bool(Category, CommandName, DefaultValue, ConsoleCommand);
}

UDebugMenuConsoleCommandInt::UDebugMenuConsoleCommandInt()
	: Super()
{
	DefaultValue = 1;
	MinValue = 0;
	MaxValue = 10;
	bExecOnLoad = false;
	bAutoApply = false;
}

void UDebugMenuConsoleCommandInt::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_Int(Category, CommandName, DefaultValue, MinValue, MaxValue, ConsoleCommand, bExecOnLoad, bAutoApply);
}

UDebugMenuConsoleCommandFloat::UDebugMenuConsoleCommandFloat()
	: Super()
{
	DefaultValue = 0.0f;
	MinValue = 0.0f;
	MaxValue = 1.0f;
	bExecOnLoad = false;
	bAutoApply = false;
}

void UDebugMenuConsoleCommandFloat::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_Float(Category, CommandName, DefaultValue, MinValue, MaxValue, ConsoleCommand, bExecOnLoad, bAutoApply);
}

UDebugMenuConsoleCommandList::UDebugMenuConsoleCommandList()
	: Super()
{
}

void UDebugMenuConsoleCommandList::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_List(Category, CommandName, Items, ConsoleCommand);
}

UDebugMenuConsoleCommandInputText::UDebugMenuConsoleCommandInputText()
	: Super()
{
}

void UDebugMenuConsoleCommandInputText::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_InputText(Category, CommandName, ConsoleCommand);
}
