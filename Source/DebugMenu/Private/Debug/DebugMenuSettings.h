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
 * Settings for the in-game debug menu.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Debug Menu"))
class UDebugMenuSettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UDebugMenuSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	/**
	 * Default category when opening the debug menu.
	 */
	UPROPERTY(EditDefaultsOnly, Category=DebugMenu)
	FName DefaultCategory = NAME_None;

	/**
	 * Allow you to define commands that should be displayed in the debug menu.
	 */
	UPROPERTY(EditDefaultsOnly, Category=DebugMenu)
	TMap<FName, FDebugMenuCategoryDefinition> CategoryToCommandsMap;
};
