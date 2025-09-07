// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

#pragma once

#include "CoreMinimal.h"

class APlayerController;

class DEBUGMENU_API IDebugMenuItemHandle
{
public:
	IDebugMenuItemHandle() = default;
	virtual ~IDebugMenuItemHandle() {}

public:
	virtual void Exec(APlayerController* PlayerController) = 0;
};

class DEBUGMENU_API IDebugMenuListHandle
{
public:
	IDebugMenuListHandle() = default;
	virtual ~IDebugMenuListHandle() {}

public:
	virtual void SetList(const TArray<FString>& List) = 0;
	virtual FString GetValue() = 0;
};

class DEBUGMENU_API IDebugMenu
{
public:
	IDebugMenu() = default;
	virtual ~IDebugMenu() {}

public:
	/**
	* Add a new item to the debug menu.
	* Category: Display the item in the tab of the debug menu with this category.
	* Label: Label to display in the debug menu for this item.
	* ConsoleCommand: This console command will be executed by this item with parameters based on its type.
	* bExecOnLoad: Execute item when value is loaded on serialize.
	*/
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_SimpleExec(const FName& Category, FString Label, FString ConsoleCommand) = 0;
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_InputText(const FName& Category, FString Label, FString ConsoleCommand) = 0;
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_Bool(const FName& Category, FString Label, bool bDefaultValue, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_Float(const FName& Category, FString Label, float DefaultValue, float MinValue, float MaxValue, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
	virtual TWeakPtr<IDebugMenuItemHandle> AddItem_Int(const FName& Category, FString Label, int32 DefaultValue, int32 MinValue, int32 MaxValue, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
	virtual TWeakPtr<IDebugMenuListHandle> AddItem_List(const FName& Category, FString Label, const TArray<FString>& ListItems, FString ConsoleCommand, bool bExecOnLoad = false) = 0;
};
