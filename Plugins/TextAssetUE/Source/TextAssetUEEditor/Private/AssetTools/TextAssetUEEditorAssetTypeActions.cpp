#include "AssetTools/TextAssetUEEditorAssetTypeActions.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"
#include "AssetTypeCategories.h"
#include "Styling/SlateBrush.h"
#include "Styles/TextAssetUEEditorStyle.h"
#include "DesktopPlatformModule.h"
#include "AssetToolsModule.h"
#include "ToolMenus.h"
#include "EditorReimportHandler.h"

FText FAssetTypeActions_TextAssetUE::GetName() const {
  return NSLOCTEXT("AssetTypeActions", "TextAssetUE", "Text Asset");
}

FColor FAssetTypeActions_TextAssetUE::GetTypeColor() const {
  return FColor::White;  //return FColor(100, 150, 255);
}

uint32 FAssetTypeActions_TextAssetUE::GetCategories() {
  return EAssetTypeCategories::Misc;
}

UClass* FAssetTypeActions_TextAssetUE::GetSupportedClass() const {
  return UTextAssetUE::StaticClass();
}

void FAssetTypeActions_TextAssetUE::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) {
  for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt) {
    if (UTextAssetUE* TextAsset = Cast<UTextAssetUE>(*ObjIt)) {
      TSharedRef<FTextAssetUEEditorToolkit> NewToolkit(new FTextAssetUEEditorToolkit());
      TArray<UObject*> ObjectsToEdit;
      ObjectsToEdit.Add(TextAsset);
      NewToolkit->InitAssetEditor(EToolkitMode::Standalone, EditWithinLevelEditor, "TextAssetUEEditorApp", FTabManager::FLayout::NullLayout, true, true, ObjectsToEdit);
    }
  }
}

const FSlateBrush* FAssetTypeActions_TextAssetUE::GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const {
  if (const FSlateBrush* SmallBrush = FTextAssetUEEditorStyle::Get().GetBrush("TextAssetUE.Thumbnail.Small")) {
    return SmallBrush;
  }
  return FTextAssetUEEditorStyle::Get().GetBrush("TextAssetUE.Thumbnail.Large");
}

void FAssetTypeActions_TextAssetUE::GetActions(const TArray<UObject*>& InObjects, FToolMenuSection& Section) {
  FAssetTypeActions_Base::GetActions(InObjects, Section);

  TArray<UTextAssetUE*> TextAssets;
  for (UObject* Obj : InObjects) {
    if (UTextAssetUE* TextAsset = Cast<UTextAssetUE>(Obj)) {
      TextAssets.Add(TextAsset);
    }
  }

  if (TextAssets.Num() > 0) {
    Section.AddMenuEntry(
      "ExportToTxt",
      NSLOCTEXT("TextAssetUE", "ExportToTxt", "Export to .txt"),
      NSLOCTEXT("TextAssetUE", "ExportToTxtTooltip", "Export the text content to a .txt file"),
      FSlateIcon(),
      FUIAction(
        FExecuteAction::CreateLambda([TextAssets]() {
          if (TextAssets.Num() != 1) {
            // Optional: show message if multiple selected
            return;
          }

          UTextAssetUE* TextAsset = TextAssets[0];
          IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
          if (DesktopPlatform) {
            FString DefaultPath = FPaths::ProjectDir();
            FString DefaultFile = TextAsset->GetName() + TEXT(".txt");
            TArray<FString> OutFilenames;

            const bool bSaved = DesktopPlatform->SaveFileDialog(
              nullptr,
              NSLOCTEXT("TextAssetUE", "ExportDialogTitle", "Export Text Asset to .txt").ToString(),
              DefaultPath,
              DefaultFile,
              TEXT("Text Files (*.txt)|*.txt"),
              EFileDialogFlags::None,
              OutFilenames
            );

            if (bSaved && OutFilenames.Num() > 0) {
              const FString& SavePath = OutFilenames[0];
              FFileHelper::SaveStringToFile(TextAsset->Content.ToString(), *SavePath);
            }
          }
          }),
        FCanExecuteAction::CreateLambda([TextAssets]() {
          return TextAssets.Num() == 1;  // keep your single-asset restriction
          })
      )
    );

    Section.AddMenuEntry(
      "Reimport",
      NSLOCTEXT("AssetTypeActions", "Reimport", "Reimport"),
      NSLOCTEXT("AssetTypeActions", "ReimportTooltip", "Reimport the text asset from its source file"),
      FSlateIcon(FAppStyle::GetAppStyleSetName(), "ContentBrowser.AssetActionReimport"),  // nice icon match
      FUIAction(
        FExecuteAction::CreateLambda([TextAssets]() {
          for (UTextAssetUE* Asset : TextAssets) {
            if (Asset) {
              FReimportManager::Instance()->Reimport(Asset);
            }
          }
          }),
        FCanExecuteAction::CreateLambda([TextAssets]() {
          return TextAssets.Num() > 0;
          })
      )
    );

    Section.AddMenuEntry(
      "ReimportFromNewFile",
      NSLOCTEXT("AssetTypeActions", "ReimportFromNewFile", "Reimport from new file..."),
      NSLOCTEXT("AssetTypeActions", "ReimportFromNewFileTooltip", "Choose a different source file and reimport the text asset"),
      FSlateIcon(),
      FUIAction(
        FExecuteAction::CreateLambda([TextAssets]() {
          IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
          if (!DesktopPlatform) {
            return;
          }

          for (UTextAssetUE* Asset : TextAssets) {
            if (!Asset || !Asset->AssetImportData) {
              continue;
            }

            // Get current path as default (or project dir if empty)
            FString DefaultPath;
            FString CurrentFilename = Asset->AssetImportData->GetFirstFilename();
            if (!CurrentFilename.IsEmpty()) {
              DefaultPath = FPaths::GetPath(CurrentFilename);
            }
            else {
              DefaultPath = FPaths::ProjectDir();
            }

            FString DefaultFile = FPaths::GetCleanFilename(CurrentFilename);
            if (DefaultFile.IsEmpty()) {
              DefaultFile = Asset->GetName() + TEXT(".txt");
            }

            TArray<FString> OutFilenames;
            const bool bOpened = DesktopPlatform->OpenFileDialog(
              nullptr,
              NSLOCTEXT("TextAssetUE", "ReimportFromNewFileDialogTitle", "Select new source .txt file").ToString(),
              DefaultPath,
              DefaultFile,
              TEXT("Text Files (*.txt)|*.txt"),
              EFileDialogFlags::None,
              OutFilenames
            );

            if (bOpened && OutFilenames.Num() > 0) {
              const FString& NewSourcePath = OutFilenames[0];

              // Update the import data (this is what "SetReimportPaths" does internally)
              Asset->AssetImportData->Update(NewSourcePath);

              // Trigger reimport using the updated path
              FReimportManager::Instance()->Reimport(Asset);
            }
          }
          }),
        FCanExecuteAction::CreateLambda([TextAssets]() {
          return TextAssets.Num() > 0;
          })
      )
    );
  }
}