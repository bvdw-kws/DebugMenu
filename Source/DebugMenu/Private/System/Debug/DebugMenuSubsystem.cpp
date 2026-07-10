// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "System/Debug/DebugMenuSubsystem.h"

#include "Debug/ImGui/ImGuiDebugMenu.h"
#include "Debug/DebugMenuConsoleCommandTypes.h"
#include "Debug/DebugMenuSettings.h"
#include "Debug/DebugMenuPreset.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

UE_DISABLE_OPTIMIZATION

UDebugMenuSubsystem::UDebugMenuSubsystem()
	: Super()
{
}

void UDebugMenuSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

#if WITH_DEBUG_MENU
	// Get debug menu configuration from project settings
	const UDebugMenuSettings* DebugMenuSettings = GetDefault<UDebugMenuSettings>();
	
	// Create the ImGui-based debug menu implementation
	ImGuiDebugMenu = NewObject<UImGuiDebugMenu>(this);
	if (ImGuiDebugMenu)
	{
		// Initialize ImGui context and register with ImGui module
		ImGuiDebugMenu->BeginSetting(GetWorld());
		
		// Set the initial category tab based on settings
		ImGuiDebugMenu->SetCategory(DebugMenuSettings->DefaultCategory);

		// Load and register the default debug preset if specified in settings
		if (!DebugMenuSettings->DefaultDebugPreset.IsNull())
		{
			UDebugMenuPreset* DefaultPreset = DebugMenuSettings->DefaultDebugPreset.LoadSynchronous();
			if (DefaultPreset)
			{
				// Register the default preset automatically at startup
				RegisterPreset(DefaultPreset);
				UE_LOG(LogTemp, Log, TEXT("DebugMenuSubsystem::Initialize - Auto-registered default preset '%s'"), 
					*DefaultPreset->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::Initialize - Failed to load default preset from settings"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("DebugMenuSubsystem::Initialize - No default preset specified in settings"));
		}
	}
#endif // WITH_DEBUG_MENU
}

void UDebugMenuSubsystem::Deinitialize()
{
	Super::Deinitialize();

#if WITH_DEBUG_MENU
	// Clean up ImGui context and unregister from ImGui module
	if (ImGuiDebugMenu)
	{
		ImGuiDebugMenu->EndSetting(GetWorld());
	}
#endif // WITH_DEBUG_MENU
}

void UDebugMenuSubsystem::Tick(float DeltaTime)
{
#if WITH_DEBUG_MENU
	QUICK_SCOPE_CYCLE_COUNTER(DebugMenu_Tick);

	if (ImGuiDebugMenu)
	{
		// Process input from the first player controller (player index 0)
		// In multiplayer scenarios, this could be extended to support multiple controllers
		APlayerController* PlayerOneController = UGameplayStatics::GetPlayerController(this, 0);
		if (IsValid(PlayerOneController))
		{
			// Check for debug menu toggle input (Ctrl+M or gamepad combination)
			if (CheckToggleDebugMenu(PlayerOneController))
			{
				bool bOpen = !ImGuiDebugMenu->IsOpenDebugMenu();
				ImGuiDebugMenu->SetOpenDebugMenu(bOpen);

				// Broadcast the toggle event for other systems to react to
				OnToggleDebugMenu.Broadcast(bOpen);
			}

			// Allow page/category navigation when menu is open
			if (IsMenuOpened())
			{
				CheckChangePage(PlayerOneController);
			}
		}
	}
#endif // WITH_DEBUG_MENU
}

TStatId UDebugMenuSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDebugMenuSubsystem, STATGROUP_Tickables);
}

void UDebugMenuSubsystem::SaveDebugMenu(TArray<uint8>& OutMemory) const
{
#if WITH_DEBUG_MENU
	if (ImGuiDebugMenu)
	{
		ImGuiDebugMenu->SaveDebug(OutMemory);
	}
#endif // WITH_DEBUG_MENU
}

void UDebugMenuSubsystem::LoadDebugMenu(const TArray<uint8>& Memory)
{
#if WITH_DEBUG_MENU
	if (ImGuiDebugMenu)
	{
		ImGuiDebugMenu->LoadDebug(Memory);
	}
#endif // WITH_DEBUG_MENU
}

IDebugMenu& UDebugMenuSubsystem::GetMutableDebugMenu()
{
#if !WITH_DEBUG_MENU
	checkNoEntry();
#endif // !WITH_DEBUG_MENU
	return *ImGuiDebugMenu;
}

bool UDebugMenuSubsystem::IsMenuOpened() const
{
#if WITH_DEBUG_MENU
	if (ImGuiDebugMenu)
	{
		return ImGuiDebugMenu->IsOpenDebugMenu();
	}
#endif // WITH_DEBUG_MENU

	return false;
}

bool UDebugMenuSubsystem::ShouldBlockPlayerControl() const
{
#if WITH_DEBUG_MENU
	if (ImGuiDebugMenu)
	{
		return ImGuiDebugMenu->ShouldBlockPlayerControl();
	}
#endif // WITH_DEBUG_MENU

	return false;
}

void UDebugMenuSubsystem::ResetDebugWindow()
{
#if WITH_DEBUG_MENU
	if (ImGuiDebugMenu)
	{
		return ImGuiDebugMenu->ResetWindow();
	}
#endif // WITH_DEBUG_MENU
}

bool UDebugMenuSubsystem::CheckChangePage(APlayerController* PlayerController)
{
	check(PlayerController != nullptr);

	if (ImGuiDebugMenu)
	{
		// Left shoulder button - navigate to previous debug category/tab
		if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftShoulder))
		{
			ImGuiDebugMenu->ChangePage(false);
			return true;
		}
		// Right shoulder button - navigate to next debug category/tab
		else if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_RightShoulder))
		{
			ImGuiDebugMenu->ChangePage(true);
			return true;
		}
	}

	return false;
}

bool UDebugMenuSubsystem::CheckToggleDebugMenu(APlayerController* PlayerController)
{
	check(PlayerController != nullptr);

	// === KEYBOARD INPUT: Ctrl + M ===
	// Track left control key state for combination input
	if (PlayerController->WasInputKeyJustPressed(EKeys::LeftControl))
	{
		bIsPressDebugMenuTriggerKey = true;
	}

	if (PlayerController->WasInputKeyJustReleased(EKeys::LeftControl))
	{
		bIsPressDebugMenuTriggerKey = false;
	}

	// If control is held, check for 'M' key press
	if (bIsPressDebugMenuTriggerKey)
	{
		if (PlayerController->WasInputKeyJustPressed(EKeys::M))
		{
			bIsPressDebugMenuShowKey = true;
		}
	}

	// === GAMEPAD INPUT: Both Thumbsticks Pressed ===
	// Track left thumbstick click state
	if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftThumbstick))
	{
		bIsHoldGamepadLeftThumbstick = true;
	}

	if (PlayerController->WasInputKeyJustReleased(EKeys::Gamepad_LeftThumbstick))
	{
		bIsHoldGamepadLeftThumbstick = false;
	}

	// Track right thumbstick click state
	if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_RightThumbstick))
	{
		bIsHoldGamepadRightThumbstick = true;
	}

	if (PlayerController->WasInputKeyJustReleased(EKeys::Gamepad_RightThumbstick))
	{
		bIsHoldGamepadRightThumbstick = false;
	}

	// Check if either input combination was triggered
	if (bIsPressDebugMenuShowKey ||
		(bIsHoldGamepadLeftThumbstick && bIsHoldGamepadRightThumbstick))
	{
		// Reset all input states after successful trigger
		bIsPressDebugMenuShowKey = false;
		bIsHoldGamepadLeftThumbstick = false;
		bIsHoldGamepadRightThumbstick = false;

		return true;
	}

	return false;
}

bool UDebugMenuSubsystem::RegisterPreset(UDebugMenuPreset* Preset)
{
#if WITH_DEBUG_MENU
	if (!Preset)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::RegisterPreset - Preset is null"));
		return false;
	}

	// Check if already registered
	if (RegisteredPresets.Contains(Preset))
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::RegisterPreset - Preset '%s' is already registered"), *Preset->GetName());
		return true;
	}

	if (!ImGuiDebugMenu)
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuSubsystem::RegisterPreset - ImGuiDebugMenu is null"));
		return false;
	}

	// Get the debug menu interface
	IDebugMenu& DebugMenu = *ImGuiDebugMenu;
	
	// Register debug commands from each category in the preset
	TArray<TWeakPtr<IDebugMenuItemHandle>> PresetHandles;
	int32 TotalCommandCount = 0;
	
	// Count total commands across all categories for reservation
	for (const auto& CategoryPair : Preset->CategoryToCommandsMap)
	{
		TotalCommandCount += CategoryPair.Value.ConsoleCommands.Num();
	}
	PresetHandles.Reserve(TotalCommandCount);
	
	// Register commands for each category
	for (const auto& CategoryPair : Preset->CategoryToCommandsMap)
	{
		const FName& CategoryName = CategoryPair.Key;
		const FDebugMenuCategoryDefinition& CategoryDef = CategoryPair.Value;
		
		for (UDebugMenuConsoleCommandBase* Command : CategoryDef.ConsoleCommands)
		{
			if (!Command)
			{
				UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::RegisterPreset - Preset '%s' category '%s' contains null command - skipping"), 
					*Preset->GetName(), *CategoryName.ToString());
				continue;
			}

			// Let each command register itself with the debug menu
			Command->RegisterConsoleCommand(CategoryName, *ImGuiDebugMenu);
			
			// Note: Current command system doesn't return handles, but we're tracking the preset
			// In a future version with proper handle return, we would store them here:
			// TWeakPtr<IDebugMenuItemHandle> Handle = Command->RegisterWithHandle(...);
			// PresetHandles.Add(Handle);
		}
	}

	// Track the preset as registered (even without handles for now)
	RegisteredPresets.Add(Preset, MoveTemp(PresetHandles));
	
	UE_LOG(LogTemp, Log, TEXT("DebugMenuSubsystem::RegisterPreset - Registered preset '%s' with %d commands across %d categories"), 
		*Preset->GetName(), TotalCommandCount, Preset->CategoryToCommandsMap.Num());
	
	return true;
#else
	UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::RegisterPreset - Debug menu disabled in this build"));
	return false;
#endif // WITH_DEBUG_MENU
}

bool UDebugMenuSubsystem::UnregisterPreset(UDebugMenuPreset* Preset)
{
#if WITH_DEBUG_MENU
	if (!Preset)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::UnregisterPreset - Preset is null"));
		return false;
	}

	// Check if the preset is registered
	if (!RegisteredPresets.Contains(Preset))
	{
		UE_LOG(LogTemp, Log, TEXT("DebugMenuSubsystem::UnregisterPreset - Preset '%s' is not registered"), *Preset->GetName());
		return false;
	}

	// Get the handles for this preset
	TArray<TWeakPtr<IDebugMenuItemHandle>>& PresetHandles = RegisteredPresets[Preset];
	
	// In a future version with proper handle support, we would cleanup each handle:
	// for (TWeakPtr<IDebugMenuItemHandle>& Handle : PresetHandles)
	// {
	//     if (Handle.IsValid())
	//     {
	//         // Call handle cleanup/unregister method
	//     }
	// }
	
	// Remove from tracking
	RegisteredPresets.Remove(Preset);
	
	UE_LOG(LogTemp, Log, TEXT("DebugMenuSubsystem::UnregisterPreset - Unregistered preset '%s'"), 
		*Preset->GetName());
	
	return true;
#else
	return false;
#endif // WITH_DEBUG_MENU
}

bool UDebugMenuSubsystem::IsPresetRegistered(UDebugMenuPreset* Preset) const
{
#if WITH_DEBUG_MENU
	if (!Preset)
	{
		return false;
	}
	
	return RegisteredPresets.Contains(Preset);
#else
	return false;
#endif // WITH_DEBUG_MENU
}

bool UDebugMenuSubsystem::SetDebugMenuCategory(const FName& CategoryName)
{
#if WITH_DEBUG_MENU
	if (!ImGuiDebugMenu)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::SetDebugMenuCategory - ImGuiDebugMenu is null"));
		return false;
	}

	if (CategoryName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::SetDebugMenuCategory - CategoryName is None"));
		return false;
	}

	// Set the category directly using ImGuiDebugMenu's SetCategory method
	ImGuiDebugMenu->SetCategory(CategoryName);
	
	UE_LOG(LogTemp, Log, TEXT("DebugMenuSubsystem::SetDebugMenuCategory - Set active category to '%s'"), *CategoryName.ToString());
	return true;
#else
	UE_LOG(LogTemp, Warning, TEXT("DebugMenuSubsystem::SetDebugMenuCategory - Debug menu disabled in this build"));
	return false;
#endif // WITH_DEBUG_MENU
}

UE_ENABLE_OPTIMIZATION
