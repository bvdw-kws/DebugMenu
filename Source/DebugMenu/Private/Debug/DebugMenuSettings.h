// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

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
