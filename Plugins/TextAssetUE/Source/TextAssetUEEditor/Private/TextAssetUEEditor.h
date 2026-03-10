#pragma once

#include "Modules/ModuleManager.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTextAssetUE, Log, All);

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
