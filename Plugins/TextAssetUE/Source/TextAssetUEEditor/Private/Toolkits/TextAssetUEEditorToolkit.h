#pragma once

#include "Toolkits/AssetEditorToolkit.h"
#include "TextAssetUE.h"
#include "Widgets/SFindReplaceDialog.h"
#include "Widgets/STextAssetEditableText.h"
#include "Widgets/SAppearanceDialog.h"
#include "LevelEditor.h"
#include "StatusBarSubsystem.h"

class FTextAssetUEEditorToolkit : public FAssetEditorToolkit {
public:
  FTextAssetUEEditorToolkit() {}
  virtual ~FTextAssetUEEditorToolkit() { FEditorDelegates::PostUndoRedo.RemoveAll(this); }

  void InitAssetEditor(const EToolkitMode::Type Mode, TSharedPtr<IToolkitHost> InitToolkitHost, FName AppIdentifier, const TSharedRef<FTabManager::FLayout>& InitLayout, bool bCreateDefaultStandaloneTab, bool bUserCreatedDocument, const TArray<UObject*>& ObjectsToEdit);

  virtual FName GetToolkitFName() const override { return "TextAssetUEEditor"; }
  virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("ToolkitName", "TextAssetUEEditor", "Text Asset UE Editor"); }
  virtual FString GetWorldCentricTabPrefix() const override { return "TextAsset"; }
  virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White; }  //return FLinearColor(0.0f, 0.5f, 1.0f, 0.5f); }
  virtual FText GetLabelForObject(UObject* Object) const;
  virtual TSharedRef<SWidget> CreateInnerWidget();

  void RefreshTextWidget();

  virtual void RegisterCommands();

  virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
  virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

  TSharedRef<SDockTab> SpawnContentTab(const FSpawnTabArgs& Args);

  // Find/Replace
  TSharedPtr<class SFindReplaceDialog> FindReplaceDialog;
  FText CurrentFindText;
  FText CurrentReplaceText;
  bool bCaseSensitive = false;
  FText GetCurrentText() const;
  void SetCurrentText(const FText& NewText);

  void OpenFindReplaceDialog();
  void PerformFindNext();
  bool PerformReplace(bool bEnableUndo = true);
  void PerformReplaceAll();
  void CloseFindReplaceDialog();

  TSharedPtr<class SAppearanceDialog> AppearanceDialog;
  void OpenAppearanceDialog();
  void CloseAppearanceDialog();

  TWeakPtr<STextAssetEditableText> TextEditorPtr;


  void ExtendToolbar();
  void FillToolbar(FToolBarBuilder& ToolbarBuilder);
  void OnFindReplaceClicked();
  void OnAppearanceClicked();
  TSharedPtr<STextBlock> GetCursorLabel() const { return CursorPositionLabel; };

private:
  TWeakObjectPtr<UTextAssetUE> EditingAsset;

  FString LastFindText;
  FString LastReplaceText;

  TSharedPtr<STextBlock> CursorPositionLabel;
  bool UpdateCursorPositionText(float DeltaTime);
};