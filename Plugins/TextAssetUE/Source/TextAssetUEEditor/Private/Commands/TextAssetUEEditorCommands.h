#pragma once

#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandList.h"

class FTextAssetUEEditorCommands : public TCommands<FTextAssetUEEditorCommands> {
public:
  FTextAssetUEEditorCommands()
    : TCommands<FTextAssetUEEditorCommands>(
        TEXT("TextAssetUEEditor"), 
        NSLOCTEXT("Contexts", "TextAssetUEEditor", "Text Asset Editor"), 
        NAME_None, 
        FAppStyle::GetAppStyleSetName()
      )
  {}

  virtual void RegisterCommands() override;

  TSharedPtr<FUICommandInfo> Find;
  TSharedPtr<FUICommandInfo> Replace;
  TSharedPtr<FUICommandInfo> FindNext;
  TSharedPtr<FUICommandInfo> FindPrevious;
};