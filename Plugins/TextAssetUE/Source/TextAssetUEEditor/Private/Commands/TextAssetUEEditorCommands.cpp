#include "TextAssetUEEditorCommands.h"
#include "Framework/Commands/UICommandList.h"

#define LOCTEXT_NAMESPACE "TextAssetUEEditorCommands"

void FTextAssetUEEditorCommands::RegisterCommands() {
  UI_COMMAND(Find, "Find", "Open find dialog", EUserInterfaceActionType::Button, FInputChord(EKeys::F, EModifierKey::Control));
  UI_COMMAND(Replace, "Replace", "Open replace dialog", EUserInterfaceActionType::Button, FInputChord(EKeys::H, EModifierKey::Control));
  UI_COMMAND(FindNext, "Find Next", "Find next occurrence", EUserInterfaceActionType::Button, FInputChord(EKeys::F3));
  UI_COMMAND(FindPrevious, "Find Previous", "Find previous occurrence", EUserInterfaceActionType::Button, FInputChord(EKeys::F3, EModifierKey::Shift));
}

#undef LOCTEXT_NAMESPACE