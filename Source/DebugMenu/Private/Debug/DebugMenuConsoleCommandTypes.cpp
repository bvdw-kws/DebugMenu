// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.


#include "DebugMenuConsoleCommandTypes.h"

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
}

void UDebugMenuConsoleCommandInt::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_Int(Category, CommandName, DefaultValue, MinValue, MaxValue, ConsoleCommand);
}

UDebugMenuConsoleCommandFloat::UDebugMenuConsoleCommandFloat()
	: Super()
{
	DefaultValue = 0.0f;
	MinValue = 0.0f;
	MaxValue = 1.0f;
}

void UDebugMenuConsoleCommandFloat::RegisterConsoleCommand(const FName& Category, UImGuiDebugMenu& DebugMenu)
{
	DebugMenu.AddItem_Float(Category, CommandName, DefaultValue, MinValue, MaxValue, ConsoleCommand);
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
