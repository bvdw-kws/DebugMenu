// Copyright (C) 2024 Van de Walle Bastien
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0


#include "ImGuiDebugMenuTypes.h"

//----------------------------------------------------------------------//
// FImGuiDebugMenuItemBase
//----------------------------------------------------------------------//
FImGuiDebugMenuItemBase::FImGuiDebugMenuItemBase()
{
}

FImGuiDebugMenuItemBase::~FImGuiDebugMenuItemBase()
{
}

void FImGuiDebugMenuItemBase::SetLabel(const FString& InLabel)
{
	Label = InLabel;
}

//----------------------------------------------------------------------//
// FImGuiDebugMenuSimpleExec
//----------------------------------------------------------------------//
FImGuiDebugMenuSimpleExec::FImGuiDebugMenuSimpleExec()
{
}

void FImGuiDebugMenuSimpleExec::Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec)
{
#if WITH_IMGUI
	ImGui::Text("%s", StringCast<ANSICHAR>(*Label).Get());
#endif // WITH_IMGUI
}

void FImGuiDebugMenuSimpleExec::Serialize_Internal(FArchive& Ar)
{
}

//----------------------------------------------------------------------//
// FImGuiDebugMenuFloat
//----------------------------------------------------------------------//
FImGuiDebugMenuFloat::FImGuiDebugMenuFloat()
{
}

void FImGuiDebugMenuFloat::Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec)
{
#if WITH_IMGUI
	ImGui::SetNextItemWidth(-(ImGui::CalcTextSize(StringCast<ANSICHAR>(*Label).Get()).x + ImGui::GetStyle().ItemInnerSpacing.x));

	if (ImGui::DragFloat(StringCast<ANSICHAR>(*Label).Get(), &Value, 0.05f, MinValue, MaxValue))
	{
		ApplyValue();
	}
#endif // WITH_IMGUI
}

void FImGuiDebugMenuFloat::Serialize_Internal(FArchive& Ar)
{
	Ar << Value;
}

void FImGuiDebugMenuFloat::ApplyValue()
{
	ReturnValue = Value;
	CommandArgument = FString::Printf(TEXT("%f"), Value);
}

//----------------------------------------------------------------------//
// FImGuiDebugMenuBool
//----------------------------------------------------------------------//
FImGuiDebugMenuBool::FImGuiDebugMenuBool()
{
}

void FImGuiDebugMenuBool::Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec)
{
#if WITH_IMGUI
	if (ImGui::Checkbox(StringCast<ANSICHAR>(*Label).Get(), &Value))
	{
		ApplyValue();
		bExec = true;
	}
#endif // WITH_IMGUI
}

void FImGuiDebugMenuBool::Serialize_Internal(FArchive& Ar)
{
	Ar << Value;
}

void FImGuiDebugMenuBool::ApplyValue()
{
	ReturnValue = Value;
	CommandArgument = FString::Printf(TEXT("%d"), ReturnValue);
}

//----------------------------------------------------------------------//
// FImGuiDebugMenuInt
//----------------------------------------------------------------------//
FImGuiDebugMenuInt::FImGuiDebugMenuInt()
{
}

void FImGuiDebugMenuInt::Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec)
{
#if WITH_IMGUI
	ImGui::SetNextItemWidth(-(ImGui::CalcTextSize(StringCast<ANSICHAR>(*Label).Get()).x + ImGui::GetStyle().ItemInnerSpacing.x));

	if (ImGui::DragInt(StringCast<ANSICHAR>(*Label).Get(), &Value, 1.0f, MinValue, MaxValue))
	{
		ApplyValue();
	}
#endif // WITH_IMGUI
}

void FImGuiDebugMenuInt::Serialize_Internal(FArchive& Ar)
{
	Ar << Value;
}

void FImGuiDebugMenuInt::ApplyValue()
{
	ReturnValue = Value;
	CommandArgument = FString::Printf(TEXT("%d"), ReturnValue);
}

//----------------------------------------------------------------------//
// FImGuiDebugMenuList
//----------------------------------------------------------------------//
FImGuiDebugMenuList::FImGuiDebugMenuList()
{
	ValueIndex = 0;
}

void FImGuiDebugMenuList::SetList(const TArray<FString>& List)
{
	SetValue(List);
}

void FImGuiDebugMenuList::Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec)
{
#if WITH_IMGUI
	ImGui::SetNextItemWidth(-(ImGui::CalcTextSize(StringCast<ANSICHAR>(*Label).Get()).x + ImGui::GetStyle().ItemInnerSpacing.x));

	const FString PreviewValue = Value.IsValidIndex(ValueIndex) ? Value[ValueIndex] : FString();
	if (ImGui::BeginCombo(StringCast<ANSICHAR>(*Label).Get(), StringCast<ANSICHAR>(*PreviewValue).Get()))
	{
		for (int32 Index = 0; Index < Value.Num(); ++Index)
		{
			bool bIsSelected = (Value[ValueIndex] == Value[Index]);
			if (ImGui::Selectable(StringCast<ANSICHAR>(*Value[Index]).Get(), bIsSelected))
			{
				ValueIndex = Index;
				ApplyValue();
				bExec = true;
			}
			if (bIsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
#endif // WITH_IMGUI
}

void FImGuiDebugMenuList::Serialize_Internal(FArchive& Ar)
{
	Ar << ValueIndex;
}

FString FImGuiDebugMenuList::GetValue()
{
	return ReturnValue;
}

void FImGuiDebugMenuList::ApplyValue()
{
	if (Value.Num())
	{
		ValueIndex = FMath::Clamp(ValueIndex, 0, Value.Num() - 1);

		if (Value.IsValidIndex(ValueIndex))
		{
			ReturnValue = Value[ValueIndex];
		}
	}
	else
	{
		ReturnValue = FString();
	}

	CommandArgument = ReturnValue;
}

//----------------------------------------------------------------------//
// FImGuiDebugMenuInputText
//----------------------------------------------------------------------//
FImGuiDebugMenuInputText::FImGuiDebugMenuInputText()
{
}

void FImGuiDebugMenuInputText::Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec)
{
#if WITH_IMGUI
	ImGui::SetNextItemWidth(-(ImGui::CalcTextSize(StringCast<ANSICHAR>(*Label).Get()).x + ImGui::GetStyle().ItemInnerSpacing.x));

	auto InputText = StringCast<ANSICHAR>(*Value);
	if (ImGui::InputText(StringCast<ANSICHAR>(*Label).Get(), (ANSICHAR*)InputText.Get(), 256))
	{
		Value = FString(InputText.Get());
		ApplyValue();
	}
#endif // WITH_IMGUI
}

void FImGuiDebugMenuInputText::Serialize_Internal(FArchive& Ar)
{
}

void FImGuiDebugMenuInputText::ApplyValue()
{
	CommandArgument = ReturnValue = Value;
}
