// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "DebugMenuPreset.h"

#include "Debug/DebugMenuConsoleCommandTypes.h"
#include "System/Debug/DebugMenuSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"

UDebugMenuPreset::UDebugMenuPreset()
{
	// Default constructor - categories are defined in CategoryToCommandsMap
}

bool UDebugMenuPreset::RegisterPreset(const UObject* WorldContext)
{
#if WITH_DEBUG_MENU
	// Get world from context
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuPreset '%s' failed to register - invalid world context"), *GetName());
		return false;
	}

	// Get the debug menu subsystem
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuPreset '%s' failed to register - no game instance"), *GetName());
		return false;
	}

	UDebugMenuSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UDebugMenuSubsystem>();
	if (!DebugSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuPreset '%s' failed to register - DebugMenuSubsystem not found"), *GetName());
		return false;
	}

	// Delegate to the subsystem for registration
	return DebugSubsystem->RegisterPreset(this);
#else
	UE_LOG(LogTemp, Warning, TEXT("DebugMenuPreset '%s' registration skipped - debug menu disabled in this build"), *GetName());
	return false;
#endif // WITH_DEBUG_MENU
}

bool UDebugMenuPreset::UnregisterPreset(const UObject* WorldContext)
{
#if WITH_DEBUG_MENU
	// Get world from context
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuPreset '%s' failed to unregister - invalid world context"), *GetName());
		return false;
	}

	// Get the debug menu subsystem
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuPreset '%s' failed to unregister - no game instance"), *GetName());
		return false;
	}

	UDebugMenuSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UDebugMenuSubsystem>();
	if (!DebugSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuPreset '%s' failed to unregister - DebugMenuSubsystem not found"), *GetName());
		return false;
	}

	// Delegate to the subsystem for unregistration
	return DebugSubsystem->UnregisterPreset(this);
#else
	return false;
#endif // WITH_DEBUG_MENU
}

bool UDebugMenuPreset::IsPresetRegistered(const UObject* WorldContext) const
{
#if WITH_DEBUG_MENU
	// Get world from context
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return false;
	}

	// Get the debug menu subsystem
	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return false;
	}

	UDebugMenuSubsystem* DebugSubsystem = GameInstance->GetSubsystem<UDebugMenuSubsystem>();
	if (!DebugSubsystem)
	{
		return false;
	}

	// Check with the subsystem
	return DebugSubsystem->IsPresetRegistered(const_cast<UDebugMenuPreset*>(this));
#else
	return false;
#endif // WITH_DEBUG_MENU
}

