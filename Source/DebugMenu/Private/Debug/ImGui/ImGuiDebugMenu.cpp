// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.


#include "ImGuiDebugMenu.h"

#include "Kismet/GameplayStatics.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"

#if WITH_IMGUI
#include "ImGuiDelegates.h"
#include "ImGuiModule.h"
#endif // WITH_IMGUI

UImGuiDebugMenu::UImGuiDebugMenu()
	: Super()
{
	ResetWindow();
}

void UImGuiDebugMenu::BeginSetting(UWorld* World)
{
#if WITH_IMGUI
	if (World != nullptr)
	{
		FImGuiDelegates::OnWorldDebug(World).AddUObject(this, &ThisClass::ImGuiTick);
	}
#endif // WITH_IMGUI
}

void UImGuiDebugMenu::EndSetting(UWorld* World)
{
#if WITH_IMGUI
	FImGuiModule::Get().GetProperties().SetInputEnabled(false);
	FImGuiModule::Get().GetProperties().SetKeyboardNavigationEnabled(false);
	FImGuiModule::Get().GetProperties().SetGamepadNavigationEnabled(false);
	FImGuiModule::Get().GetProperties().SetGamepadInputShared(false);

	if (World != nullptr)
	{
		FImGuiDelegates::OnWorldDebug(World).RemoveAll(this);
	}
#endif // WITH_IMGUI
}

void UImGuiDebugMenu::ImGuiTick()
{
	if (IsOpenDebugMenu())
	{
		bool bIsOpenDebugMenuNew = bIsOpenDebugMenu;
		DrawImGui(bIsOpenDebugMenuNew);

		if (bIsOpenDebugMenuNew != bIsOpenDebugMenu)
		{
			SetOpenDebugMenu(bIsOpenDebugMenuNew);
		}
	}
}

void UImGuiDebugMenu::ResetWindow()
{
#if WITH_IMGUI
	WindowPos = ImVec2(60.0f, 60.0f);
	WindowSize = ImVec2(400.0f, 400.0f);
	bResetWindow = true;
#endif // WITH_IMGUI
}

void UImGuiDebugMenu::SetOpenDebugMenu(bool bInIsOpenDebugMenu)
{
	if (bIsOpenDebugMenu != bInIsOpenDebugMenu)
	{
		bIsOpenDebugMenu = bInIsOpenDebugMenu;

#if WITH_IMGUI
		FImGuiModule::Get().GetProperties().SetInputEnabled(bIsOpenDebugMenu);
		FImGuiModule::Get().GetProperties().SetKeyboardNavigationEnabled(bIsOpenDebugMenu);
		FImGuiModule::Get().GetProperties().SetGamepadNavigationEnabled(bIsOpenDebugMenu);

		FImGuiModule::Get().GetProperties().SetGamepadInputShared(bIsOpenDebugMenu);
#endif // WITH_IMGUI
	}
}

void UImGuiDebugMenu::ChangePage(bool bNext)
{
	if (bNowSearch)
	{
		return;
	}

	int32 NowPageIndex = FMath::Max(0, GetCategoryIndex(NowPage));
	bNext ? ++NowPageIndex : --NowPageIndex;
	int32 NumPage = static_cast<int32>(DebugCategories.Num());
	if (NowPageIndex >= NumPage)
	{
		NowPageIndex = 0;
	}
	else if (NowPageIndex < 0)
	{
		NowPageIndex = NumPage - 1;
	}
	SetCategoryByIndex(NowPageIndex);
}

int32 UImGuiDebugMenu::GetCategoryIndex(const FName& Category) const
{
	int32 CategoryNum = static_cast<int32>(DebugCategories.Num());
	for (int32 CategoryIndex = 0; CategoryIndex < CategoryNum; ++CategoryIndex)
	{
		if (DebugCategories[CategoryIndex].CategoryName == Category)
		{
			return CategoryIndex;
		}
	}

	return INDEX_NONE;
}

void UImGuiDebugMenu::SetCategoryByIndex(int32 Index)
{
	if (DebugCategories.IsValidIndex(Index))
	{
		NowPage = DebugCategories[Index].CategoryName;
	}
	else
	{
		NowPage = NAME_None;
	}
}

void UImGuiDebugMenu::DrawImGui(bool& bOutIsOpenDebugMenu)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	bool bIsPressedPrevButton = false;
	bool bIsPressedNextButton = false;

#if WITH_IMGUI
	FString CategoryName = NowPage.ToString();
	if (bNowSearch)
	{
		CategoryName = "SEARCH";
	}

	const ImGuiCond WindowCond = bResetWindow ? 0 : ImGuiCond_FirstUseEver;
	bResetWindow = false;

	ImGui::SetNextWindowPos(WindowPos, WindowCond);
	ImGui::SetNextWindowSize(WindowSize, WindowCond);
	ImGui::Begin("Debug Menu", &bOutIsOpenDebugMenu);

	// Header
	const float WindowWidth = ImGui::GetWindowWidth();
	const float ButtonWidth = WindowWidth * (1.0f / 5.0f);
	const float TitleWidth = WindowWidth * (3.0f / 5.0f);

	float TextWidth = 0.0f;
	float Offset = 0.0f;
	int ColumnIndex = 0;
	ImGui::Columns(3, "Index", false);
	{
		ImGui::SetColumnWidth(ColumnIndex++, ButtonWidth);
		const FString ButtonText = "Prev";
		TextWidth = ImGui::CalcTextSize(StringCast<ANSICHAR>(*ButtonText).Get()).x;
		Offset = (ButtonWidth - TextWidth) * 0.5f;
		ImGui::SameLine(Offset);
		if (ImGui::Button(StringCast<ANSICHAR>(*ButtonText).Get()))
		{
			bIsPressedPrevButton = true;
		}
	}
	ImGui::NextColumn();
	{
		ImGui::SetColumnWidth(ColumnIndex++, TitleWidth);
		ImGui::SetColumnWidth(ColumnIndex++, ButtonWidth);

		TextWidth = ImGui::CalcTextSize(StringCast<ANSICHAR>(*CategoryName).Get()).x;
		Offset = (TitleWidth - TextWidth) * 0.5f;
		ImGui::SameLine(Offset);
		ImGui::Text("%s", StringCast<ANSICHAR>(*CategoryName).Get());
	}
	ImGui::NextColumn();
	{
		const FString ButtonText = "Next";
		TextWidth = ImGui::CalcTextSize(StringCast<ANSICHAR>(*ButtonText).Get()).x;
		Offset = (ButtonWidth - TextWidth) * 0.5f;
		ImGui::SameLine(Offset);
		if (ImGui::Button(StringCast<ANSICHAR>(*ButtonText).Get()))
		{
			bIsPressedNextButton = true;
		}
	}
	ImGui::Columns();

	// Page
	if (ImGui::CollapsingHeader("Pages"))
	{
		float LineWidth = 0.0f;
		float TextMargin = 50.0f;
		float ButtonPadding = ImGui::GetStyle().FramePadding.x;

		int32 CategoryNum = static_cast<int32>(DebugCategories.Num());
		for (int32 CategoryIndex = 0; CategoryIndex < CategoryNum; ++CategoryIndex)
		{
			const FName& Category = DebugCategories[CategoryIndex].CategoryName;
			const FString CategoryText = Category.ToString();

			TextWidth = ImGui::CalcTextSize(StringCast<ANSICHAR>(*CategoryText).Get()).x + ButtonPadding;

			bool bNewLine = true;
			if (CategoryIndex != 0)
			{
				bNewLine = LineWidth + TextWidth + TextMargin > WindowWidth;
				if (bNewLine)
				{
					ImGui::Columns();
				}
				else
				{
					ImGui::SameLine();
				}
			}

			if (ImGui::Button(StringCast<ANSICHAR>(*CategoryText).Get()))
			{
				NowPage = DebugCategories[CategoryIndex].CategoryName;
			}

			if (bNewLine)
			{
				LineWidth = ImGui::GetItemRectSize().x;
			}
			else
			{
				LineWidth += ImGui::GetItemRectSize().x + ButtonPadding;
			}

			if (CategoryIndex + 1 == CategoryNum)
			{
				ImGui::Columns();
			}
		}
	}

	// Search
	const FString SearchLabel = TEXT("SEARCH");

	ANSICHAR SearchBuff[256];
	SearchBuff[0] = '\0';
	FCStringAnsi::Strcpy(SearchBuff, StringCast<ANSICHAR>(*MySearch).Get());

	if (ImGui::InputText(StringCast<ANSICHAR>(*SearchLabel).Get(), SearchBuff, sizeof(SearchBuff)))
	{
		SearchList.Reset();
		MySearch = SearchBuff;
		
		const FString SearchText = FString::Printf(TEXT(".*%s.*"), *MySearch);

		for (int32 CategoryIndex = 0; CategoryIndex < DebugCategories.Num(); ++CategoryIndex)
		{
			for (int32 ItemIndex = 0; ItemIndex < DebugCategories[CategoryIndex].ChildItems.Num(); ++ItemIndex)
			{
				if (FImGuiDebugMenuItemBase* MenuBase = DebugCategories[CategoryIndex].ChildItems[ItemIndex].Get())
				{
					if (MenuBase->GetLabel().Contains(MySearch))
					{
						SearchList.Add(DebugCategories[CategoryIndex].ChildItems[ItemIndex]);
					}
				}
			}
		}
	}

	if (MySearch.Len() != 0)
	{
		bNowSearch = true;
	}
	else
	{
		bNowSearch = false;
		SearchList.Reset();
	}

	ImGui::Separator();
	ImGui::Spacing();

	// Draw
	const int32 NowPageIndex = GetCategoryIndex(NowPage);
	if (bNowSearch)
	{
		DrawParams.Page = NowPageIndex;

		for (int32 ItemIndex = 0; ItemIndex < SearchList.Num(); ++ItemIndex)
		{
			DrawParams.Index = ItemIndex;

			if (SearchList[ItemIndex].IsValid())
			{
				SearchList[ItemIndex]->Draw(PlayerController, DrawParams);
			}
		}
	}
	else
	{
		DrawParams.Page = NowPageIndex;

		if (DebugCategories.IsValidIndex(NowPageIndex))
		{
			for (int32 ItemIndex = 0; ItemIndex < DebugCategories[NowPageIndex].ChildItems.Num(); ++ItemIndex)
			{
				DrawParams.Index = ItemIndex;

				DebugCategories[NowPageIndex].ChildItems[ItemIndex]->Draw(PlayerController, DrawParams);
			}
		}
	}

	WindowPos = ImGui::GetWindowPos();
	WindowSize = ImGui::GetWindowSize();
	ImGui::End();
#endif // WITH_IMGUI

	if (bIsPressedPrevButton || bIsPressedNextButton)
	{
		ChangePage(bIsPressedNextButton);
	}
}

void UImGuiDebugMenu::SerializeDebug(FArchive& Ar)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	for (const auto& DebugCategory : DebugCategories)
	{
		for (const auto& Item : DebugCategory.ChildItems)
		{
			if (!Item.IsValid())
			{
				continue;
			}

			Item->Serialize(Ar, PlayerController);
		}
	}
}

void UImGuiDebugMenu::SaveDebug(TArray<uint8>& OutMemory) const
{
	FMemoryWriter MemoryWriter(OutMemory);
	const_cast<UImGuiDebugMenu*>(this)->SerializeDebug(MemoryWriter);
}

void UImGuiDebugMenu::LoadDebug(const TArray<uint8>& Memory)
{
	FMemoryReader MemoryReader(Memory);
	SerializeDebug(MemoryReader);
}

template<typename T>
TWeakPtr<T> UImGuiDebugMenu::AddItem_Template(
	const FName& Category,
	const FString& Label,
	const FString& ConsoleCommand,
	bool bExecOnLoad /*= false*/)
{
	int32 CategoryIndex = GetCategoryIndex(Category);
	if (CategoryIndex == INDEX_NONE)
	{
		CategoryIndex = DebugCategories.Num();
		for (int32 OldCategoryIndex = 0; OldCategoryIndex < DebugCategories.Num(); OldCategoryIndex++)
		{
			if (Category.ToString() < DebugCategories[OldCategoryIndex].CategoryName.ToString())
			{
				CategoryIndex = OldCategoryIndex;
				break;
			}
		}
		FImGuiDebugMenuCategoryItem& NewCategorItem = DebugCategories.InsertDefaulted_GetRef(CategoryIndex);
		NewCategorItem.CategoryName = Category;
	}

	TSharedPtr<T> NewItem = MakeShared<T>();
	if (NewItem.IsValid())
	{
		NewItem->SetLabel(Label);
		NewItem->SetShouldExecOnLoad(bExecOnLoad);
		NewItem->SetCommand(ConsoleCommand);

		DebugCategories[CategoryIndex].ChildItems.Add(NewItem);

		return NewItem;
	}

	return nullptr;
}

TWeakPtr<IDebugMenuItemHandle> UImGuiDebugMenu::AddItem_SimpleExec(
	const FName& Category,
	FString Label,
	FString ConsoleCommand)
{
	return AddItem_Template<FImGuiDebugMenuSimpleExec>(Category, Label, ConsoleCommand);
}

TWeakPtr<IDebugMenuItemHandle> UImGuiDebugMenu::AddItem_Bool(
	const FName& Category,
	FString Label,
	bool bDefaultValue,
	FString ConsoleCommand,
	bool bExecOnLoad /*= false*/)
{
	TWeakPtr<FImGuiDebugMenuBool> NewItem = AddItem_Template<FImGuiDebugMenuBool>(Category, Label, ConsoleCommand, bExecOnLoad);

	if (NewItem.IsValid())
	{
		NewItem.Pin()->SetValue(bDefaultValue);
	}

	return NewItem;
}

TWeakPtr<IDebugMenuItemHandle> UImGuiDebugMenu::AddItem_Float(
	const FName& Category,
	FString Label,
	float DefaultValue,
	float MinValue,
	float MaxValue,
	FString ConsoleCommand,
	bool bExecOnLoad /*= false*/)
{
	TWeakPtr<FImGuiDebugMenuFloat> NewItem = AddItem_Template<FImGuiDebugMenuFloat>(Category, Label, ConsoleCommand, bExecOnLoad);

	if (NewItem.IsValid())
	{
		NewItem.Pin()->SetValue(DefaultValue);
		NewItem.Pin()->SetValueRange(MinValue, MaxValue);
	}

	return NewItem;
}

TWeakPtr<IDebugMenuItemHandle> UImGuiDebugMenu::AddItem_Int(
	const FName& Category,
	FString Label,
	int32 DefaultValue,
	int32 MinValue,
	int32 MaxValue,
	FString ConsoleCommand,
	bool bExecOnLoad /*= false*/)
{
	TWeakPtr<FImGuiDebugMenuInt> NewItem = AddItem_Template<FImGuiDebugMenuInt>(Category, Label, ConsoleCommand, bExecOnLoad);

	if (NewItem.IsValid())
	{
		NewItem.Pin()->SetValue(DefaultValue);
		NewItem.Pin()->SetValueRange(MinValue, MaxValue);
	}

	return NewItem;
}

TWeakPtr<IDebugMenuListHandle> UImGuiDebugMenu::AddItem_List(
	const FName& Category,
	FString Label,
	const TArray<FString>& ListItems,
	FString ConsoleCommand,
	bool bExecOnLoad /*= false*/)
{
	TWeakPtr<FImGuiDebugMenuList> NewItem = AddItem_Template<FImGuiDebugMenuList>(Category, Label, ConsoleCommand, bExecOnLoad);

	if (NewItem.IsValid())
	{
		NewItem.Pin()->SetValue(ListItems);
	}

	return NewItem;
}

TWeakPtr<IDebugMenuItemHandle> UImGuiDebugMenu::AddItem_InputText(
	const FName& Category,
	FString Label,
	FString ConsoleCommand)
{
	return AddItem_Template<FImGuiDebugMenuInputText>(Category, Label, ConsoleCommand);
}
