// Copyright (C) 2024 Van de Walle Bastien
// All Rights Reserved.
//
// This software is licensed for use in Unreal Engine projects only.
// Redistribution or sharing of source code is prohibited.
// See LICENSE file for complete terms.


#include "DebugMenuActor.h"

#include "Debug/DebugMenuPreset.h"
#include "Components/BillboardComponent.h"
#include "System/Debug/DebugMenuSubsystem.h"

ADebugMenuActor::ADebugMenuActor()
{
	// This actor doesn't need to tick - it only registers/unregisters during lifecycle events
	PrimaryActorTick.bCanEverTick = false;

	// Create a billboard component for visibility in the editor
	// This helps level designers find and select debug menu actors
	UBillboardComponent* BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = BillboardComponent;
	
	// Set up the billboard with a debug-appropriate icon
	// In a real implementation, you might want to create a custom icon texture
	BillboardComponent->SetHiddenInGame(true); // Only visible in editor
	BillboardComponent->SetUsingAbsoluteScale(true);
}

void ADebugMenuActor::BeginPlay()
{
	Super::BeginPlay();

#if WITH_DEBUG_MENU
	// Only register if we have a valid preset and auto-registration is enabled
	if (bAutoRegisterOnBeginPlay)
	{
		if (RegisterDebugMenu())
		{
			UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' auto-registered preset '%s' on BeginPlay"), 
				*GetName(), DebugMenuPreset ? *DebugMenuPreset->GetName() : TEXT("None"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' failed to auto-register preset on BeginPlay"), 
				*GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' skipped auto-registration (disabled)"), 
			*GetName());
	}
#endif // WITH_DEBUG_MENU
}

void ADebugMenuActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#if WITH_DEBUG_MENU
	// Always attempt cleanup on EndPlay to prevent stale debug menu entries
	if (UnregisterDebugMenu())
	{
		UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' unregistered preset '%s' on EndPlay"), 
			*GetName(), DebugMenuPreset ? *DebugMenuPreset->GetName() : TEXT("None"));
	}
#endif // WITH_DEBUG_MENU

	Super::EndPlay(EndPlayReason);
}

bool ADebugMenuActor::RegisterDebugMenu()
{
#if WITH_DEBUG_MENU
	// Validate that we have a preset to register
	if (!DebugMenuPreset)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' cannot register - no preset assigned"), 
			*GetName());
		return false;
	}

	// Prevent duplicate registrations
	if (bIsCurrentlyRegistered)
	{
		UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' preset already registered"), 
			*GetName());
		return true;
	}

	// Delegate to the preset's registration logic (pass this actor as world context)
	bool bRegistrationSuccess = DebugMenuPreset->RegisterPreset(this);
	
	if (bRegistrationSuccess)
	{
		bIsCurrentlyRegistered = true;
		
		UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' successfully registered preset '%s'"), 
			*GetName(), *DebugMenuPreset->GetName());

		// If force open is enabled, switch to the first available category from this preset
		if (bForceOpenPresetCategory)
		{
			if (UDebugMenuSubsystem* DebugMenuSubsystem = GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>())
			{
				// Get the first category from the preset's CategoryToCommandsMap
				if (DebugMenuPreset->CategoryToCommandsMap.Num() > 0)
				{
					FName FirstCategory = DebugMenuPreset->CategoryToCommandsMap.begin()->Key;
					if (DebugMenuSubsystem->SetDebugMenuCategory(FirstCategory))
					{
						UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' forced open first category '%s'"), 
							*GetName(), *FirstCategory.ToString());
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' failed to force open category '%s'"), 
							*GetName(), *FirstCategory.ToString());
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' cannot force open category - preset has no categories defined"), 
						*GetName());
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' cannot force open category - DebugMenuSubsystem not found"), 
					*GetName());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DebugMenuActor '%s' failed to register preset '%s'"), 
			*GetName(), *DebugMenuPreset->GetName());
	}

	return bRegistrationSuccess;
#else
	// Debug menu system disabled in this build
	UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' registration skipped - debug menu disabled"), 
		*GetName());
	return false;
#endif // WITH_DEBUG_MENU
}

bool ADebugMenuActor::UnregisterDebugMenu()
{
#if WITH_DEBUG_MENU
	// Nothing to unregister if not currently registered
	if (!bIsCurrentlyRegistered)
	{
		return false;
	}

	// We need a valid preset to unregister
	if (!DebugMenuPreset)
	{
		// This shouldn't happen, but handle gracefully
		bIsCurrentlyRegistered = false;
		return false;
	}

	// Delegate to the preset's unregistration logic (pass this actor as world context)
	bool bUnregistrationSuccess = DebugMenuPreset->UnregisterPreset(this);
	
	if (bUnregistrationSuccess)
	{
		bIsCurrentlyRegistered = false;
		
		UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' successfully unregistered preset '%s'"), 
			*GetName(), *DebugMenuPreset->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' failed to unregister preset '%s'"), 
			*GetName(), *DebugMenuPreset->GetName());
		
		// Reset state anyway to prevent issues
		bIsCurrentlyRegistered = false;
	}

	return bUnregistrationSuccess;
#else
	// Reset state in non-debug builds
	bIsCurrentlyRegistered = false;
	return false;
#endif // WITH_DEBUG_MENU
}

bool ADebugMenuActor::IsDebugMenuRegistered() const
{
	// Check both our tracking state and the preset's state for accuracy
	return bIsCurrentlyRegistered && 
		   DebugMenuPreset && 
		   DebugMenuPreset->IsPresetRegistered(this);
}

void ADebugMenuActor::SetDebugMenuPreset(UDebugMenuPreset* NewPreset)
{
	// If we're changing presets and currently registered, unregister the old one first
	if (bIsCurrentlyRegistered && DebugMenuPreset && DebugMenuPreset != NewPreset)
	{
		UnregisterDebugMenu();
	}

	// Assign the new preset
	DebugMenuPreset = NewPreset;

	// If auto-registration is enabled and we're already in play, register the new preset
	if (bAutoRegisterOnBeginPlay && HasActorBegunPlay() && NewPreset)
	{
		RegisterDebugMenu();
	}

	UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' preset changed to '%s'"), 
		*GetName(), NewPreset ? *NewPreset->GetName() : TEXT("None"));
}

bool ADebugMenuActor::OpenPresetCategory()
{
#if WITH_DEBUG_MENU
	// Validate that we have a preset to get the category from
	if (!DebugMenuPreset)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' cannot open category - no preset assigned"), 
			*GetName());
		return false;
	}

	// Check if preset has any categories defined
	if (DebugMenuPreset->CategoryToCommandsMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' cannot open category - preset has no categories defined"), 
			*GetName());
		return false;
	}

	// Get the debug menu subsystem
	UDebugMenuSubsystem* DebugMenuSubsystem = GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>();
	if (!DebugMenuSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' cannot open category - DebugMenuSubsystem not found"), 
			*GetName());
		return false;
	}

	// Switch to the first category from the preset
	FName FirstCategory = DebugMenuPreset->CategoryToCommandsMap.begin()->Key;
	bool bSuccess = DebugMenuSubsystem->SetDebugMenuCategory(FirstCategory);
	
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("DebugMenuActor '%s' opened first category '%s'"), 
			*GetName(), *FirstCategory.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' failed to open category '%s'"), 
			*GetName(), *FirstCategory.ToString());
	}

	return bSuccess;
#else
	UE_LOG(LogTemp, Warning, TEXT("DebugMenuActor '%s' cannot open category - debug menu disabled in this build"), 
		*GetName());
	return false;
#endif // WITH_DEBUG_MENU
}