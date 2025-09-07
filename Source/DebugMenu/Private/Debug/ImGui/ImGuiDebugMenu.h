// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ImGuiDebugMenuTypes.h"
#include "Debug/DebugMenuInterface.h"

#include "ImGuiDebugMenu.generated.h"

UCLASS()
class DEBUGMENU_API UImGuiDebugMenu : public UObject, public IDebugMenu
{
	GENERATED_BODY()

	UImGuiDebugMenu();

public:
	void BeginSetting(UWorld* World);
	void EndSetting(UWorld* World);

	void SetCategory(const FName& InCategory) { NowPage = InCategory; }
	void ChangePage(bool bNext);

	void SetOpenDebugMenu(bool bInIsOpenDebugMenu);
	bool IsOpenDebugMenu() const { return bIsOpenDebugMenu; }

	void ResetWindow();

	void SaveDebug(TArray<uint8>& OutMemory) const;
	void LoadDebug(const TArray<uint8>& Memory);
	void SerializeDebug(FArchive& Ar);

	// Todo: Callback when an item is triggered so we can queue it in the replay system

public:
	TWeakPtr<IDebugMenuItemHandle> AddItem_SimpleExec(const FName& Category, FString Label, FString ConsoleCommand) override final;
	TWeakPtr<IDebugMenuItemHandle> AddItem_InputText(const FName& Category, FString Label, FString ConsoleCommand) override final;
	TWeakPtr<IDebugMenuItemHandle> AddItem_Bool(const FName& Category, FString Label, bool bDefaultValue, FString ConsoleCommand, bool bExecOnLoad = false) override final;
	TWeakPtr<IDebugMenuItemHandle> AddItem_Float(const FName& Category, FString Label, float DefaultValue, float MinValue, float MaxValue, FString ConsoleCommand, bool bExecOnLoad = false) override final;
	TWeakPtr<IDebugMenuItemHandle> AddItem_Int(const FName& Category, FString Label, int32 DefaultValue, int32 MinValue, int32 MaxValue, FString ConsoleCommand, bool bExecOnLoad = false) override final;
	TWeakPtr<IDebugMenuListHandle> AddItem_List(const FName& Category, FString Label, const TArray<FString>& ListItems, FString ConsoleCommand, bool bExecOnLoad = false) override final;

protected:
	template<typename T>
	TWeakPtr<T> AddItem_Template(const FName& CategoryPage, const FString& Label, const FString& ConsoleCommand, bool bExecOnLoad = false);

protected:
	UFUNCTION()
	void ImGuiTick();
	
private:
	UPROPERTY(Transient)
	bool bIsOpenDebugMenu{ false };
	UPROPERTY(Transient)
	TArray<FImGuiDebugMenuCategoryItem> DebugCategories;

	UPROPERTY(Transient)
	bool bNowSearch{ false };
	UPROPERTY(Transient)
	FName NowPage{ NAME_None };

#if WITH_IMGUI
	ImVec2 WindowPos;
	ImVec2 WindowSize;
	bool bResetWindow{ false };
#endif // WITH_IMGUI
	FImGuiDebugMenuItemDrawParams DrawParams;

	FString MySearch;
	TArray<TSharedPtr<FImGuiDebugMenuItemBase>> SearchList;

	UPROPERTY(Transient)
	bool bAutoSaveWhenCloseDebugMenu{ true };

	void DrawImGui(bool& bOutIsOpenDebugMenu);
	int32 GetCategoryIndex(const FName& Category) const;
	void SetCategoryByIndex(int32 Index);

};
