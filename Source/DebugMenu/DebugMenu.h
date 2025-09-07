// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

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
