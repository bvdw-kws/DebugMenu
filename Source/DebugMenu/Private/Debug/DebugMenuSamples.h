// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "CoreMinimal.h"

/**
 * Debug Menu Samples - Sample console variables demonstrating debug menu functionality.
 * 
 * This file provides example console variables of different types that can be used
 * to demonstrate and test the Debug Menu plugin's capabilities. These variables
 * are accessible via console commands and can be integrated into Blueprint graphs
 * for runtime debugging and testing.
 * 
 * Available Sample Variables:
 * - debugMenu.sample.booleanVariable  (bool)   - Boolean toggle example
 * - debugMenu.sample.integerVariable  (int32)  - Integer value example  
 * - debugMenu.sample.floatVariable    (float)  - Float value example
 * - debugMenu.sample.stringVariable   (string) - String value example
 * 
 * These variables serve as practical examples for:
 * - Console variable implementation patterns
 * - Debug menu integration testing
 * - Blueprint console command demonstrations (via GetConsoleVariableFloatValue, etc.)
 * - Runtime debugging workflows
 * 
 * Implementation Notes:
 * - Uses TAutoConsoleVariable<T> for automatic registration and Blueprint compatibility
 * - Console variables are accessible from both C++ and Blueprint
 * - Values can be changed via console commands or debug menu controls
 */