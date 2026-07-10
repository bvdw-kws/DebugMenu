// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#include "DebugMenuSamples.h"
#include "Engine/Engine.h"
#include "HAL/IConsoleManager.h"

// Sample Boolean Variable using TAutoConsoleVariable for Blueprint compatibility
static TAutoConsoleVariable<bool> CVarDebugMenuSampleBooleanVariable(
	TEXT("debugMenu.sample.booleanVariable"),
	false,
	TEXT("Sample boolean variable for demonstrating debug menu boolean controls.\n")
	TEXT("Usage: debugMenu.sample.booleanVariable [0|1]\n")
	TEXT("Default: false (0)\n")
	TEXT("Accessible in Blueprint via GetConsoleVariableBoolValue"),
	ECVF_Default
);

// Sample Integer Variable using TAutoConsoleVariable for Blueprint compatibility
static TAutoConsoleVariable<int32> CVarDebugMenuSampleIntegerVariable(
	TEXT("debugMenu.sample.integerVariable"),
	42,
	TEXT("Sample integer variable for demonstrating debug menu integer controls.\n")
	TEXT("Usage: debugMenu.sample.integerVariable [value]\n")
	TEXT("Default: 42\n")
	TEXT("Accessible in Blueprint via GetConsoleVariableIntValue"),
	ECVF_Default
);

// Sample Float Variable using TAutoConsoleVariable for Blueprint compatibility
static TAutoConsoleVariable<float> CVarDebugMenuSampleFloatVariable(
	TEXT("debugMenu.sample.floatVariable"),
	3.14f,
	TEXT("Sample float variable for demonstrating debug menu float controls.\n")
	TEXT("Usage: debugMenu.sample.floatVariable [value]\n")
	TEXT("Default: 3.14\n")
	TEXT("Accessible in Blueprint via GetConsoleVariableFloatValue"),
	ECVF_Default
);

// Sample String Variable using TAutoConsoleVariable for Blueprint compatibility
static TAutoConsoleVariable<FString> CVarDebugMenuSampleStringVariable(
	TEXT("debugMenu.sample.stringVariable"),
	TEXT("Hello World"),
	TEXT("Sample string variable for demonstrating debug menu string controls.\n")
	TEXT("Usage: debugMenu.sample.stringVariable [text]\n")
	TEXT("Default: \"Hello World\"\n")
	TEXT("Accessible in Blueprint via GetConsoleVariableStringValue"),
	ECVF_Default
);