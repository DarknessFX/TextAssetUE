#pragma once

#include "Modules/ModuleManager.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"

class FTextAssetUEEditorModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

  void RegisterToolbar();
  void OnFindReplaceClicked();
  void OnAppearanceClicked();

  TSharedPtr<STextBlock> GetCursorLabel() const { return CursorPositionLabel; };

private:
  FTextAssetUEEditorToolkit* FindActiveTextAssetToolkit();
  TSharedPtr<STextBlock> CursorPositionLabel;

  bool UpdateCursorPositionText(float DeltaTime);
};
