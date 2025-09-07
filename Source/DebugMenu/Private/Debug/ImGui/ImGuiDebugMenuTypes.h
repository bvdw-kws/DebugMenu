// Copyright (C) 2024 Van de Walle Bastien                                                                                                                                             │ │
// All Rights Reserved.                                                                                                                                                                │ │
//                                                                                                                                                                                     │ │
// This software is licensed for use in Unreal Engine projects only.                                                                                                                   │ │
// Redistribution or sharing of source code is prohibited.                                                                                                                             │ │
// See LICENSE file for complete terms.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiCommon.h"
#include "GameFramework/PlayerController.h"
#include "Debug/DebugMenuInterface.h"

#include "ImGuiDebugMenuTypes.generated.h"

template<typename T>
using ImGuiDebugMenuFunction = bool (*)(T);

USTRUCT()
struct DEBUGMENU_API FImGuiDebugMenuCommand
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category=Command)
	FString ConsoleCommand;
};

struct FImGuiDebugMenuItemDrawParams
{
	int32 Page{ 0 };
	int32 Index{ 0 };
};

class IImGuiDebugMenuItemHandle
{
public:
	IImGuiDebugMenuItemHandle() = default;
	virtual ~IImGuiDebugMenuItemHandle() {}

public:
	virtual void SetShouldExecOnLoad(bool bInExecOnLoad) = 0;
	virtual void Serialize(FArchive& Ar, APlayerController* PlayerController) = 0;

	virtual void SetCommand(const FString& InCommand) = 0;
};

class FImGuiDebugMenuItemBase : public IImGuiDebugMenuItemHandle, public IDebugMenuItemHandle
{
public:
	FImGuiDebugMenuItemBase();
	virtual ~FImGuiDebugMenuItemBase();

public:
	virtual void Draw(APlayerController* PlayerController, const FImGuiDebugMenuItemDrawParams& InDrawParams) {}

	void Exec(APlayerController* PlayerController) override final
	{
		ExecCommand(PlayerController);
	}

public:
	void SetLabel(const FString& InLabel);
	FString GetLabel() { return Label; }

protected:
	virtual void ExecCommand(APlayerController* PlayerController) = 0;

protected:
	FString Label;

};
template<typename DataType, typename ReturnType=DataType>
class FImGuiDebugMenuItemTemplateBase : public FImGuiDebugMenuItemBase
{
public:
	FImGuiDebugMenuItemTemplateBase() {}
	virtual ~FImGuiDebugMenuItemTemplateBase() {}

public:
	void SetValue(DataType InValue)
	{
		Value = InValue;
		ApplyValue();
	}

	void SetValueRange(DataType InMinValue, DataType InMaxValue)
	{
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}

	void SetFunction(ImGuiDebugMenuFunction<void*> InFunction)
	{
		Function = InFunction;
	}

public:
	void SetCommand(const FString& InCommand) override final
	{
		ConsoleCommand = InCommand;
	}

	void SetShouldExecOnLoad(bool bInExecOnLoad) override final
	{
		bExecOnLoad = bInExecOnLoad;
	}

	void SetAutoApply(bool bInAutoApply)
	{
		bAutoApply = bInAutoApply;
	}

	bool GetAutoApply() const
	{
		return bAutoApply;
	}

	virtual void Draw(APlayerController* PlayerController, const FImGuiDebugMenuItemDrawParams& InDrawParams) override final
	{
#if WITH_IMGUI
		bool bExec = false;
		bool bIsForcused = false;

		ImGui::Columns(2, nullptr, false);
		{
			ImGui::SetColumnWidth(-1, 50.f);

			const FString ButtonId = FString::Printf(TEXT("Exec##Exec%d_%d"), InDrawParams.Page, InDrawParams.Index);
			if (ImGui::Button(StringCast<ANSICHAR>(*ButtonId).Get()))
			{
				bExec = true;
			}
			ImGui::NextColumn();
		}

		if (ImGui::IsItemFocused())
		{
			bIsForcused = true;
		}

		Draw_Internal(InDrawParams, bIsForcused, bExec);

		ImGui::Columns();

		if (bExec)
		{
			ExecCommand(PlayerController);
		}
#endif // WITH_IMGUI
	}

	void Serialize(FArchive& Ar, APlayerController* PlayerController) override final
	{
		Serialize_Internal(Ar);
		ApplyValue();

		if (Ar.IsLoading() && bExecOnLoad)
		{
			ExecCommand(PlayerController);
		}
	}

protected:
	virtual void Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec) = 0;
	virtual void Serialize_Internal(FArchive& Ar) = 0;

	virtual void ApplyValue() {};

protected:
	void ExecCommand(APlayerController* PlayerController) override final
	{
		if (IsValid(PlayerController) && ConsoleCommand.IsEmpty() == false)
		{
			FString Command = ConsoleCommand;
			if (CommandArgument.IsEmpty() == false)
			{
				Command += (" " + CommandArgument);
			}
			PlayerController->ConsoleCommand(Command);
		}

		if (Function != nullptr)
		{
			Function(ReturnValue);
		}
	}

protected:
	FString CommandArgument;
	FString ConsoleCommand;

	ReturnType ReturnValue;
	ImGuiDebugMenuFunction<ReturnType> Function{ nullptr };

	DataType Value;
	DataType MinValue;
	DataType MaxValue;

private:
	bool bExecOnLoad = true;
	bool bAutoApply = false;

};

class FImGuiDebugMenuSimpleExec : public FImGuiDebugMenuItemTemplateBase<void*>
{
public:
	FImGuiDebugMenuSimpleExec();
	~FImGuiDebugMenuSimpleExec() {};

	// FImGuiDebugMenuItemTemplateBase implementation Begin
public:
	virtual void Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec) override;
	virtual void Serialize_Internal(FArchive& Ar) override;
	// FImGuiDebugMenuItemTemplateBase implementation End

};

class FImGuiDebugMenuFloat : public FImGuiDebugMenuItemTemplateBase<float>
{
public:
	FImGuiDebugMenuFloat();
	~FImGuiDebugMenuFloat() {};

	// FImGuiDebugMenuItemTemplateBase implementation Begin
protected:
	virtual void Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec) override;
	virtual void Serialize_Internal(FArchive& Ar) override;
	virtual void ApplyValue() override;
	// FImGuiDebugMenuItemTemplateBase implementation End
};

class FImGuiDebugMenuBool : public FImGuiDebugMenuItemTemplateBase<bool>
{
public:
	FImGuiDebugMenuBool();
	~FImGuiDebugMenuBool() {};

	// FImGuiDebugMenuItemTemplateBase implementation Begin
protected:
	virtual void Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec) override;
	virtual void Serialize_Internal(FArchive& Ar) override;
	virtual void ApplyValue() override;
	// FImGuiDebugMenuItemTemplateBase implementation End
};

class FImGuiDebugMenuInt : public FImGuiDebugMenuItemTemplateBase<int32>
{
public:
	FImGuiDebugMenuInt();
	~FImGuiDebugMenuInt() {};

	// FImGuiDebugMenuItemTemplateBase implementation Begin
protected:
	virtual void Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec) override;
	virtual void Serialize_Internal(FArchive& Ar) override;
	virtual void ApplyValue() override;
	// FImGuiDebugMenuItemTemplateBase implementation End
};

class FImGuiDebugMenuList : public FImGuiDebugMenuItemTemplateBase<TArray<FString>, FString>, public IDebugMenuListHandle
{
public:
	FImGuiDebugMenuList();
	~FImGuiDebugMenuList() {};

	// IImGuiDebugMenuListItemHandle implementation Begin
public:
	virtual void SetList(const TArray<FString>& List) override;
	virtual FString GetValue() override;
	// IImGuiDebugMenuListItemHandle implementation End

	// FImGuiDebugMenuItemTemplateBase implementation Begin
protected:
	virtual void Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec) override;
	virtual void Serialize_Internal(FArchive& Ar) override;
	virtual void ApplyValue() override;
	// FImGuiDebugMenuItemTemplateBase implementation End

private:
	int32 ValueIndex;

};

class FImGuiDebugMenuInputText : public FImGuiDebugMenuItemTemplateBase<FString>
{
public:
	FImGuiDebugMenuInputText();
	~FImGuiDebugMenuInputText() {};

	// FImGuiDebugMenuItemTemplateBase implementation Begin
protected:
	virtual void Draw_Internal(const FImGuiDebugMenuItemDrawParams& InDrawParams, bool& bIsForcused, bool& bExec) override;
	virtual void Serialize_Internal(FArchive& Ar) override;
	virtual void ApplyValue() override;
	// FImGuiDebugMenuItemTemplateBase implementation End

};

USTRUCT()
struct FImGuiDebugMenuCategoryItem
{
    GENERATED_BODY()

	FName CategoryName = NAME_None;
    TArray<TSharedPtr<FImGuiDebugMenuItemBase>> ChildItems;
};
