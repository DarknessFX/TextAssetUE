     .----------------.  .----------------.  .----------------. 
    | .--------------. || .--------------. || .--------------. |
    | |  ________    | || |  _________   | || |  ____  ____  | |
    | | |_   ___ `.  | || | |_   ___  |  | || | |_  _||_  _| | |
    | |   | |   `. \ | || |   | |_  \_|  | || |   \ \  / /   | |
    | |   | |    | | | || |   |  _|      | || |    > `' <    | |
    | |  _| |___.' / | || |  _| |_       | || |  _/ /'`\ \_  | |
    | | |________.'  | || | |_____|      | || | |____||____| | |
    | |              | || |              | || |              | |
    | '--------------' || '--------------' || '--------------' |
     '----------------'  '----------------'  '----------------' 

           DarknessFX @ https://dfx.lv | X: @DrkFX

# TextAssetUE

## About

**Modern Unreal Engine 5 port of the classic TextAsset plugin**, this is a complete rewrite and update of the beloved **TextAsset** plugin by ue4plugins (https://github.com/ue4plugins/TextAsset) latest release was in 2018 for UE4.19.

The original plugin was simple, lightweight, and extremely useful: a Notepad-like text asset right inside Unreal Engine for quick notes, TODO lists, references, links, code snippets, credits lists, etc.

**TextAssetUE** brings it back to life for Unreal Engine 5 (tested on 5.7.3), with modern C++ code, clean structure, and improved editor integration.

## Features

- Create .txt-like assets directly in Content Browser
- Full notepad editor inside UE (double-click TextAsset uasset)
- Multi-line editable text with undo/redo
- Save/reload content automatically
- Import .txt files via drag & drop or Import button
- Export to .txt via right-click → Export to .txt
- Customizable appearance (Editor Preferences → Plugins → Text Asset UE):
  - Inherit background/text color from UE theme (default on)
  - Custom background & text color
  - Font family picker
  - Font size
  - Text margin/padding around the editor area
  - Word wrap toggle

## Commentary

I kept the TextAsset plugin alive through multiple Unreal Engine versions, until recently when I rebuilt UE5.7.3 and deleted the Plugins folder, forgetting to save my copy of the TextAsset plugin... Thanks to the interest of dev friends who also like the plugin ( https://x.com/DrkFX/status/2028496389588557828 ) I took the time to re-write this plugin for Unreal Engine 5.

## Credits & Thanks

TextAssetUE plugin exists thanks to the original **TextAsset** by **ue4plugins** (https://github.com/ue4plugins/TextAsset).

Huge respect and gratitude to the author for creating such a useful plugin in the UE4 days.

## Run as Sample uproject

1. Download or clone this repo
2. Right-click the TextAssetUE_Sample.uproject and Generate Project Files.
3. Open the TextAssetUE_Sample.sln
4. Run the project.

## Installation

1. Download or clone this repo
2. Copy the `Plugins/TextAssetUE` folder to your project's `Plugins/` directory
3. Enable the plugin in your .uproject file or via Edit → Plugins
4. Right-click the TextAssetUE_Sample.uproject and Generate Project Files.
5. Open your project solution file (.sln)
5. Run the project.

## License

@MIT - Free for everyone and any use. <br/><br/>
DarknessFX @ <a href="https://dfx.lv" target="_blank">https://dfx.lv</a> | X: <a href="https://twitter.com/DrkFX" target="_blank">@DrkFX</a> <br/>https://github.com/DarknessFX/TextAssetUE
