// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.


#include "System/Debug/DebugMenuSubsystem.h"

#include "Debug/ImGui/ImGuiDebugMenu.h"
#include "Debug/DebugMenuConsoleCommandTypes.h"
#include "Debug/DebugMenuSettings.h"
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

		// Register all predefined console commands from settings
		for (const auto& CategoryToCommandDefs : DebugMenuSettings->CategoryToCommandsMap)
		{
			for (const auto& ConsoleCommandDef : CategoryToCommandDefs.Value.ConsoleCommands)
			{
				if (ConsoleCommandDef == nullptr)
				{
					continue;
				}

				// Each console command definition knows how to register itself with the debug menu
				ConsoleCommandDef->RegisterConsoleCommand(CategoryToCommandDefs.Key, *ImGuiDebugMenu);
			}
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

UE_ENABLE_OPTIMIZATION
