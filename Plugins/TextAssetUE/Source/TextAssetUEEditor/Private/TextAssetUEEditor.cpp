#include "TextAssetUEEditor.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetTools/TextAssetUEEditorAssetTypeActions.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"
#include "Styles/TextAssetUEEditorStyle.h"
#include "Settings/TextAssetUEEditorSettings.h"
#include "Settings/TextAssetUEEditorCustomization.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "TextAssetUEEditor"

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
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTextAssetUEEditorModule, TextAssetUEEditor)