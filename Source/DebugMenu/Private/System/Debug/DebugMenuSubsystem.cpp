// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


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
	const UDebugMenuSettings* DebugMenuSettings = GetDefault<UDebugMenuSettings>();
	
	ImGuiDebugMenu = NewObject<UImGuiDebugMenu>(this);
	if (ImGuiDebugMenu)
	{
		ImGuiDebugMenu->BeginSetting(GetWorld());
		ImGuiDebugMenu->SetCategory(DebugMenuSettings->DefaultCategory);

		for (const auto& CategoryToCommandDefs : DebugMenuSettings->CategoryToCommandsMap)
		{
			for (const auto& ConsoleCommandDef : CategoryToCommandDefs.Value.ConsoleCommands)
			{
				if (ConsoleCommandDef == nullptr)
				{
					continue;
				}

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
		APlayerController* PlayerOneController = UGameplayStatics::GetPlayerController(this, 0);
		if (IsValid(PlayerOneController))
		{
			if (CheckToggleDebugMenu(PlayerOneController))
			{
				bool bOpen = !ImGuiDebugMenu->IsOpenDebugMenu();
				ImGuiDebugMenu->SetOpenDebugMenu(bOpen);

				OnToggleDebugMenu.Broadcast(bOpen);
			}

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
		if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftShoulder))
		{
			ImGuiDebugMenu->ChangePage(false);
			return true;
		}
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

	// Keyboard
	if (PlayerController->WasInputKeyJustPressed(EKeys::LeftControl))
	{
		bIsPressDebugMenuTriggerKey = true;
	}

	if (PlayerController->WasInputKeyJustReleased(EKeys::LeftControl))
	{
		bIsPressDebugMenuTriggerKey = false;
	}

	if (bIsPressDebugMenuTriggerKey)
	{
		if (PlayerController->WasInputKeyJustPressed(EKeys::M))
		{
			bIsPressDebugMenuShowKey = true;
		}
	}

	// Gamepad
	if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftThumbstick))
	{
		bIsHoldGamepadLeftThumbstick = true;
	}

	if (PlayerController->WasInputKeyJustReleased(EKeys::Gamepad_LeftThumbstick))
	{
		bIsHoldGamepadLeftThumbstick = false;
	}

	if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_RightThumbstick))
	{
		bIsHoldGamepadRightThumbstick = true;
	}

	if (PlayerController->WasInputKeyJustReleased(EKeys::Gamepad_RightThumbstick))
	{
		bIsHoldGamepadRightThumbstick = false;
	}

	if (bIsPressDebugMenuShowKey ||
		(bIsHoldGamepadLeftThumbstick && bIsHoldGamepadRightThumbstick))
	{
		bIsPressDebugMenuShowKey = false;

		bIsHoldGamepadLeftThumbstick = false;
		bIsHoldGamepadRightThumbstick = false;

		return true;
	}

	return false;
}

UE_ENABLE_OPTIMIZATION
