// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "Modules/ModuleManager.h"

/**
 * Debug Menu Module - Main module class for the Debug Menu plugin.
 * 
 * This module handles the initialization and shutdown of the debug menu system.
 * It provides a lightweight module interface that integrates with Unreal Engine's
 * module loading system.
 * 
 * The module is configured to load early in the engine startup process to ensure
 * debug menu functionality is available as soon as possible for debugging during
 * development.
 * 
 * Key Responsibilities:
 * - Module lifecycle management (startup/shutdown)
 * - Integration with UE's module system
 * - Early initialization for debug functionality
 * 
 * The actual debug menu functionality is implemented in the UDebugMenuSubsystem,
 * which is automatically created and managed by Unreal Engine's subsystem framework.
 */
class DEBUGMENU_API FDebugMenu : public IModuleInterface
{
};
