// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "Engine/World.h"

#include "DebugMenuSubsystem.generated.h"

class IDebugMenu;
class APlayerController;

DECLARE_MULTICAST_DELEGATE_OneParam(FDebugMenuToggleSignature, bool /*bOpen*/);

#ifndef WITH_DEBUG_MENU
#define WITH_DEBUG_MENU 0
#endif // WITH_DEBUG_MENU

UCLASS()
class DEBUGMENU_API UDebugMenuSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UDebugMenuSubsystem();

	bool IsMenuOpened() const;
	void ResetDebugWindow();

	void SaveDebugMenu(TArray<uint8>& OutMemory) const;
	void LoadDebugMenu(const TArray<uint8>& Memory);

	IDebugMenu& GetMutableDebugMenu();
	
	FDebugMenuToggleSignature OnToggleDebugMenu;

protected:
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	// FTickableGameObject implementation Begin
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// FTickableGameObject implementation End
	
private:
	UPROPERTY(Transient)
	TObjectPtr<class UImGuiDebugMenu> ImGuiDebugMenu;
	
	UPROPERTY(Transient)
	bool bIsPressDebugMenuTriggerKey{ false };
	UPROPERTY(Transient)
	bool bIsHoldGamepadLeftThumbstick{ false };
	UPROPERTY(Transient)
	bool bIsHoldGamepadRightThumbstick{ false };
	UPROPERTY(Transient)
	bool bIsPressDebugMenuShowKey{ false };

	bool CheckToggleDebugMenu(APlayerController* PlayerController);
	bool CheckChangePage(APlayerController* PlayerController);

};
