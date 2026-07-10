// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "Debug/DebugMenuSettings.h"

#include "UObject/ConstructorHelpers.h"

UDebugMenuSettings::UDebugMenuSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDebugMenuPreset> DebugMenuPresetAsset(TEXT("/DebugMenu/DataAsset/DA_DebugMenu_Preset_Stat"));
	DefaultDebugPreset = DebugMenuPresetAsset.Object;
}
