#pragma once

#include "Toolkits/AssetEditorToolkit.h"
#include "TextAssetUE.h"

class FTextAssetUEEditorToolkit : public FAssetEditorToolkit {
public:
  FTextAssetUEEditorToolkit() {}
  virtual ~FTextAssetUEEditorToolkit() {}

  void InitAssetEditor(const EToolkitMode::Type Mode, TSharedPtr<IToolkitHost> InitToolkitHost, FName AppIdentifier, const TSharedRef<FTabManager::FLayout>& InitLayout, bool bCreateDefaultStandaloneTab, bool bUserCreatedDocument, const TArray<UObject*>& ObjectsToEdit);

  virtual FName GetToolkitFName() const override { return "TextAssetUEEditor"; }
  virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("ToolkitName", "TextAssetUEEditor", "Text Asset UE Editor"); }
  virtual FString GetWorldCentricTabPrefix() const override { return "TextAsset"; }
  virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::White; }  //return FLinearColor(0.0f, 0.5f, 1.0f, 0.5f); }
  virtual FText GetLabelForObject(UObject* Object) const;
  virtual TSharedRef<SWidget> CreateInnerWidget();

  virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
  virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

  TSharedRef<SDockTab> SpawnContentTab(const FSpawnTabArgs& Args);
private:
  TWeakObjectPtr<UTextAssetUE> EditingAsset;
};