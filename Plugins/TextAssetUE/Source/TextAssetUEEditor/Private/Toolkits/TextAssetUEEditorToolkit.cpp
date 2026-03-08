#include "TextAssetUEEditorToolkit.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "Settings/TextAssetUEEditorSettings.h"
#include "Engine/Font.h"

void FTextAssetUEEditorToolkit::InitAssetEditor(const EToolkitMode::Type Mode, TSharedPtr<IToolkitHost> InitToolkitHost, FName AppIdentifier, const TSharedRef<FTabManager::FLayout>& InitLayout, bool bCreateDefaultStandaloneTab, bool bUserCreatedDocument, const TArray<UObject*>& ObjectsToEdit) {
  EditingAsset = Cast<UTextAssetUE>(ObjectsToEdit[0]);

  const TSharedRef<FTabManager::FLayout> DefaultLayout = FTabManager::NewLayout("TextAssetUE_Layout_v1")
    ->AddArea(
      FTabManager::NewPrimaryArea()
      ->Split(
        FTabManager::NewStack()
        ->AddTab("TextAssetContentTab", ETabState::OpenedTab)
        ->SetForegroundTab(FTabId("TextAssetContentTab"))
        ->SetHideTabWell(true)
      )
    );

  FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, AppIdentifier, DefaultLayout, bCreateDefaultStandaloneTab, bUserCreatedDocument, ObjectsToEdit);
}

void FTextAssetUEEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
  FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

  InTabManager->RegisterTabSpawner("TextAssetContentTab", FOnSpawnTab::CreateSP(this, &FTextAssetUEEditorToolkit::SpawnContentTab))
    .SetDisplayName(NSLOCTEXT("TextAssetUE", "ContentTab", "TextAsset"));
    //.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FTextAssetUEEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
  FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
  InTabManager->UnregisterTabSpawner("TextAssetContentTab");
}

TSharedRef<SDockTab> FTextAssetUEEditorToolkit::SpawnContentTab(const FSpawnTabArgs& Args) {
  return SNew(SDockTab)
    .TabRole(ETabRole::PanelTab)
    [
      CreateInnerWidget()
    ];
}

TSharedRef<SWidget> FTextAssetUEEditorToolkit::CreateInnerWidget() {
  const UTextAssetUEEditorSettings* Settings = GetDefault<UTextAssetUEEditorSettings>();

  FSlateFontInfo FontInfo;
  if (UFont* LoadedFont = Settings->FontAsset.LoadSynchronous()) {
    FontInfo = LoadedFont->GetLegacySlateFontInfo();
  } else {
    FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", Settings->FontSize);
  }
  FontInfo.Size = Settings->FontSize;

  FLinearColor BgColor = Settings->bInheritBackground ? FAppStyle::GetColor("ToolPanel.GroupBorder") : Settings->BackgroundColor;
  FLinearColor TxtColor = Settings->bInheritTextColor ? FAppStyle::GetColor("NormalForeground") : Settings->TextColor;

  return SNew(SBorder)
    .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
    .BorderBackgroundColor(BgColor)
    [
      SNew(SMultiLineEditableText)
        .Text_Lambda([this]() -> FText {
          return EditingAsset.IsValid() ? EditingAsset->Content : FText::GetEmpty();
        })
        .OnTextChanged_Lambda([this](const FText& NewText) {
          if (EditingAsset.IsValid()) {
            EditingAsset->Modify();
            EditingAsset->Content = NewText;
          }
        })
        .AllowMultiLine(true)
        .IsReadOnly(false)
        .Font(FontInfo)
        .Margin(Settings->TextMargin)
    ];
}

FText FTextAssetUEEditorToolkit::GetLabelForObject(UObject* Object) const {
  if (UTextAssetUE* TextAsset = Cast<UTextAssetUE>(Object)) {
    return FText::FromString(TextAsset->GetName());
  }
  return FText::FromName(Object->GetFName());
}
