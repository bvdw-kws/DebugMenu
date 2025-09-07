# Debug Menu Plugin for Unreal Engine

[![License](https://img.shields.io/badge/license-Commercial-blue.svg)](LICENSE)
[![Engine](https://img.shields.io/badge/Unreal%20Engine-5.4%2B-orange.svg)](https://www.unrealengine.com)
[![ImGui](https://img.shields.io/badge/ImGui-1.74-green.svg)](https://github.com/ocornut/imgui)

**Professional runtime debug menus for Unreal Engine projects**

Create powerful, categorized debug interfaces with real-time parameter tweaking for rapid iteration and debugging. Perfect for developers, QA teams, and anyone who needs runtime control over game parameters.

![Debug Menu Preview](Screenshots/debug_menu_preview.png)
*Runtime debug menu with categorized tabs and various control types*

## ✨ Features

### 🎛️ **Rich Control Types**
- **Bool toggles** - Checkboxes for boolean values
- **Float sliders** - Min/max constrained numerical input  
- **Integer spinners** - Whole number controls with bounds
- **Input text fields** - String parameter entry
- **Dropdown lists** - Selection from predefined options
- **Simple buttons** - Execute console commands instantly

### 📋 **Organized Interface**
- **Tabbed categories** - Group related controls logically
- **Search functionality** - Quickly find specific parameters
- **Save/Load settings** - Persistent debug configurations
- **Multi-PIE support** - Works seamlessly with multiple game instances

### 🛠️ **Developer Friendly**
- **Console command integration** - All controls execute UE console commands
- **C++ and Blueprint support** - Easy integration from any codebase
- **Runtime only** - Automatically excluded from shipping builds
- **Gamepad navigation** - Full controller support for console development

## 📥 Installation

1. **Download** the Debug Menu plugin from the Unreal Engine Marketplace
2. **Enable** the plugin in your project's Plugin Browser
3. **Add dependency** to your module's `Build.cs` file:

```csharp
if (Target.Configuration != UnrealTargetConfiguration.Shipping)
{
    PrivateDependencyModuleNames.Add("DebugMenu");
}
```

## 🚀 Quick Start

### C++ Usage

```cpp
#include "System/Debug/DebugMenuSubsystem.h"

// Get the debug menu subsystem
UDebugMenuSubsystem* DebugMenuSubsystem = GetGameInstance()->GetSubsystem<UDebugMenuSubsystem>();
IDebugMenu& DebugMenu = DebugMenuSubsystem->GetMutableDebugMenu();

// Add various control types
DebugMenu.AddItem_Bool(TEXT("Player"), TEXT("God Mode"), false, TEXT("god"));
DebugMenu.AddItem_Float(TEXT("Player"), TEXT("Speed Multiplier"), 1.0f, 0.1f, 5.0f, TEXT("slomo"));
DebugMenu.AddItem_List(TEXT("Graphics"), TEXT("View Mode"), 
    {TEXT("Lit"), TEXT("Unlit"), TEXT("Wireframe")}, TEXT("viewmode"));
```

### Blueprint Usage

Use the **Get Debug Menu Subsystem** node to access the debug menu system from Blueprint:

![Blueprint Usage](Screenshots/blueprint_usage.png)

### Opening the Debug Menu

- **Default key binding**: `F1` (configurable)
- **Console command**: `showdebugmenu`
- **Gamepad**: Hold both thumbsticks and press any face button

## 📖 API Reference

### Control Types

#### `AddItem_Bool(Category, Label, DefaultValue, ConsoleCommand, ExecOnLoad)`
Creates a checkbox control that executes a console command with 0/1 parameter.

#### `AddItem_Float(Category, Label, DefaultValue, MinValue, MaxValue, ConsoleCommand, ExecOnLoad)`  
Creates a slider control with min/max bounds that passes the float value to a console command.

#### `AddItem_Int(Category, Label, DefaultValue, MinValue, MaxValue, ConsoleCommand, ExecOnLoad)`
Creates an integer spinner with bounds that passes the integer value to a console command.

#### `AddItem_InputText(Category, Label, ConsoleCommand)`
Creates a text input field that passes the entered string to a console command.

#### `AddItem_List(Category, Label, ListItems, ConsoleCommand, ExecOnLoad)`
Creates a dropdown list that passes the selected item string to a console command.

#### `AddItem_SimpleExec(Category, Label, ConsoleCommand)`
Creates a button that executes a console command without parameters.

### Parameters

- **Category**: Tab name where the control appears (creates tab if doesn't exist)
- **Label**: Display name for the control
- **ConsoleCommand**: UE console command to execute when control changes
- **ExecOnLoad**: Whether to execute the command when loading saved settings
- **DefaultValue/MinValue/MaxValue**: Numerical constraints for sliders and spinners
- **ListItems**: Array of strings for dropdown options

## ⚙️ Configuration

### Input Settings

Configure debug menu controls in **Project Settings > Plugins > Debug Menu**:

- **Toggle Key**: Keyboard shortcut to open/close debug menu
- **Gamepad Navigation**: Enable controller support for menus
- **Auto-Save**: Automatically save debug settings when closing menu

### Custom Categories

Organize your debug controls with meaningful category names:

```cpp
// Group related functionality
DebugMenu.AddItem_Bool(TEXT("Physics"), TEXT("Show Collision"), false, TEXT("show collision"));
DebugMenu.AddItem_Bool(TEXT("Physics"), TEXT("Freeze Physics"), false, TEXT("physics freeze"));

DebugMenu.AddItem_Float(TEXT("Rendering"), TEXT("Screen Percentage"), 100.0f, 25.0f, 200.0f, TEXT("r.ScreenPercentage"));
DebugMenu.AddItem_Bool(TEXT("Rendering"), TEXT("Show FPS"), false, TEXT("stat fps"));
```

## 💡 Best Practices

### Performance Considerations
- **Conditional compilation**: Wrap debug menu code in `#if !UE_BUILD_SHIPPING` blocks
- **Lazy initialization**: Create debug menu items during gameplay, not at startup
- **Reasonable bounds**: Set appropriate min/max values for numerical controls

### Organization Tips
- **Logical grouping**: Use categories that match your game's systems
- **Clear labels**: Use descriptive names that indicate what each control does  
- **Consistent naming**: Follow a consistent naming convention across categories

### Console Command Integration
```cpp
// Create console variables that work with debug menu
static TAutoConsoleVariable<bool> CVarShowDebugInfo(
    TEXT("game.ShowDebugInfo"), false,
    TEXT("Show debug information overlay"));

// Add to debug menu
DebugMenu.AddItem_Bool(TEXT("Debug"), TEXT("Show Debug Info"), false, TEXT("game.ShowDebugInfo"));
```

## 🔧 Advanced Usage

### Save/Load Debug Settings

```cpp
// Save current debug menu state
TArray<uint8> SaveData;
DebugMenuSubsystem->SaveDebugMenu(SaveData);

// Load previously saved state  
DebugMenuSubsystem->LoadDebugMenu(SaveData);
```

### Dynamic Control Updates

```cpp
// Get handle to modify control later
TWeakPtr<IDebugMenuListHandle> ListHandle = DebugMenu.AddItem_List(
    TEXT("Dynamic"), TEXT("Available Levels"), 
    {TEXT("Level1"), TEXT("Level2")}, TEXT("open"));

// Update the list dynamically
if (ListHandle.IsValid())
{
    TArray<FString> NewLevels = GetAvailableLevels();
    ListHandle.Pin()->SetList(NewLevels);
}
```

## 🎮 Multi-PIE Support

The Debug Menu plugin automatically handles multiple Play-In-Editor instances:

- Each PIE instance gets its own debug menu context
- Settings can be configured independently per instance  
- Perfect for testing multiplayer scenarios with different debug states

## 📋 Requirements

- **Unreal Engine**: 5.4 or later
- **Project Type**: C++ project (Blueprint projects supported with C++ modules)
- **Build Configuration**: Development or DebugGame (automatically excluded from Shipping)

## 🐛 Troubleshooting

### Debug Menu Won't Open
- Ensure plugin is enabled in Plugin Browser
- Check that you're running Development or DebugGame build
- Verify input binding in Project Settings

### Controls Don't Work
- Confirm console commands are valid and properly spelled
- Check console for error messages when controls are activated
- Ensure console variables exist and are accessible

### Build Errors
- Add `DebugMenu` to your module's dependency list
- Wrap debug menu code in `#if !UE_BUILD_SHIPPING` conditionals
- Include proper header files: `#include "System/Debug/DebugMenuSubsystem.h"`

## 📄 License

This plugin is licensed for commercial use. See [LICENSE](LICENSE) file for complete terms.

**Third-party components:**
- ImGui library: MIT License (see Source/ImGui/LICENSE)

## 🙋‍♂️ Support

For questions, bug reports, or feature requests, please use the support channels provided with this plugin on the Unreal Engine Marketplace.

---

*Made with ❤️ for the Unreal Engine developer community*