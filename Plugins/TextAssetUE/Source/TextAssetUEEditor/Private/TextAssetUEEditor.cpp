#include "TextAssetUEEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetTools/TextAssetUEEditorAssetTypeActions.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"
#include "Styles/TextAssetUEEditorStyle.h"
#include "Settings/TextAssetUEEditorSettings.h"
#include "Settings/TextAssetUEEditorCustomization.h"
#include "Factories/TextAssetUEEditorFactory.h"
#include "ISettingsModule.h"
#include "EditorReimportHandler.h"
#include "ToolMenus.h"
#include "Containers/Ticker.h"
#include "Widgets/Input/SNumericEntryBox.h"

DEFINE_LOG_CATEGORY(LogTextAssetUE);
#define LOCTEXT_NAMESPACE "TextAssetUEEditor"

FTSTicker::FDelegateHandle TickHandle;

void FTextAssetUEEditorModule::StartupModule() {
  IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
  TSharedRef<FAssetTypeActions_TextAssetUE> Actions = MakeShareable(new FAssetTypeActions_TextAssetUE());
  AssetTools.RegisterAssetTypeActions(Actions);
  FTextAssetUEEditorStyle::Get();
  if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
    SettingsModule->RegisterSettings(
      "Editor",
      "Plugins",
      "TextAssetUE",
      NSLOCTEXT("TextAssetUE", "TextAssetUESettingsName", "TextAssetUE"),
      NSLOCTEXT("TextAssetUE", "TextAssetUESettingsDescription", "Customize TextAssetUE editor appearance"),
      GetMutableDefault<UTextAssetUEEditorSettings>()
    );
  }
  if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor")) {
    PropertyModule->RegisterCustomClassLayout(
      UTextAssetUEEditorSettings::StaticClass()->GetFName(),
      FOnGetDetailCustomizationInstance::CreateStatic(&FTextAssetUEEditorCustomization::MakeInstance)
    );
  }
  UTextAssetUEEditorFactory* FactoryForReimport = NewObject<UTextAssetUEEditorFactory>();
  FReimportManager::Instance()->RegisterHandler(*FactoryForReimport);

  UToolMenus::RegisterStartupCallback(
    FSimpleMulticastDelegate::FDelegate::CreateRaw(
      this,
      &FTextAssetUEEditorModule::RegisterToolbar
    )
  );

  TickHandle = FTSTicker::GetCoreTicker().AddTicker(
    FTickerDelegate::CreateRaw(this, &FTextAssetUEEditorModule::UpdateCursorPositionText),
    0.5f
  );
}

void FTextAssetUEEditorModule::ShutdownModule() {
  if (FModuleManager::Get().IsModuleLoaded("AssetTools")) {
    IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetTools.UnregisterAssetTypeActions(MakeShareable(new FAssetTypeActions_TextAssetUE()));
  }
  if (FModuleManager::Get().IsModuleLoaded("Settings")) {
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
      SettingsModule->UnregisterSettings("Editor", "Plugins", "TextAssetUE");
    }
  }
  if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor")) {
    PropertyModule->UnregisterCustomClassLayout(UTextAssetUEEditorSettings::StaticClass()->GetFName());
  }
  UToolMenus::UnRegisterStartupCallback(this);
  UToolMenus::UnregisterOwner(this);
  FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
}

void FTextAssetUEEditorModule::RegisterToolbar() {
  FToolMenuOwnerScoped OwnerScoped(this);

  UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("AssetEditor.DefaultToolBar");
  FToolMenuSection& Section = Menu->AddSection("TextAssetTools");

  Section.AddEntry(
    FToolMenuEntry::InitToolBarButton(
      "TextAssetUE.FindReplaceButton",
      FUIAction(
        FExecuteAction::CreateRaw(
          this,
          &FTextAssetUEEditorModule::OnFindReplaceClicked
        )
      ),
      LOCTEXT("FindReplaceBtn", "Find / Replace"),
      LOCTEXT("FindReplaceTooltip", "Open find and replace dialog"),
      FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Search")
    )
  );

  Section.AddEntry(FToolMenuEntry::InitSeparator("TextAssetUE.Separator1"));

  Section.AddEntry(
    FToolMenuEntry::InitToolBarButton(
      "TextAssetUE.AppearanceButton",
      FUIAction(
        FExecuteAction::CreateRaw(
          this,
          &FTextAssetUEEditorModule::OnAppearanceClicked
        )
      ),
      LOCTEXT("AppearanceBtn", "Appearance"),
      LOCTEXT("AppearanceTooltip", "Open the text appearance dialog"),
      FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings")
    )
  );

  Section.AddEntry(FToolMenuEntry::InitSeparator("TextAssetUE.Separator2"));

  Section.AddEntry(
    FToolMenuEntry::InitWidget(
      "TextAssetUE.CursorPositionLabel",
      SNew(SBox)
      .HAlign(HAlign_Right)
      .VAlign(VAlign_Center)
      [
        SAssignNew(CursorPositionLabel, STextBlock)
          .Text(LOCTEXT("CursorPos", "Ln 1, Col 1"))
      ],
      LOCTEXT("CursorPosLabel", "Cursor Position")
    )
  );
}

void FTextAssetUEEditorModule::OnFindReplaceClicked() {
  if (FTextAssetUEEditorToolkit* Toolkit = FindActiveTextAssetToolkit()) {
    Toolkit->OpenFindReplaceDialog();
  }
}

void FTextAssetUEEditorModule::OnAppearanceClicked() {
  if (FTextAssetUEEditorToolkit* Toolkit = FindActiveTextAssetToolkit()) {
    Toolkit->OpenAppearanceDialog();
  }
}


bool FTextAssetUEEditorModule::UpdateCursorPositionText(float DeltaTime) {
  if (!CursorPositionLabel.IsValid()) { return true; }

  if (FTextAssetUEEditorToolkit* Toolkit = FindActiveTextAssetToolkit()) {
    if (TSharedPtr<STextAssetEditableText> Editor = Toolkit->TextEditorPtr.Pin()) {
      CursorPositionLabel->SetText(Editor->GetCursorPositionText());
    }
  }

  return true;
}

FTextAssetUEEditorToolkit* FTextAssetUEEditorModule::FindActiveTextAssetToolkit() {
  if (!GEditor) return nullptr;

  UAssetEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
  if (!Subsystem) return nullptr;

  for (UObject* Asset : Subsystem->GetAllEditedAssets()) {
    IAssetEditorInstance* EditorInstance = Subsystem->FindEditorForAsset(Asset, false);
    if (!EditorInstance) continue;
    FTextAssetUEEditorToolkit* Toolkit = static_cast<FTextAssetUEEditorToolkit*>(EditorInstance);
    if (Toolkit->GetTabManager()->GetOwnerTab()->IsForeground()) {
      return Toolkit;
    }
  }

  return nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTextAssetUEEditorModule, TextAssetUEEditor)