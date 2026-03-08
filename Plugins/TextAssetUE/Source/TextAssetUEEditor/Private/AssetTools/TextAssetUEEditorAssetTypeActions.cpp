#include "AssetTools/TextAssetUEEditorAssetTypeActions.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"
#include "AssetTypeCategories.h"
#include "Styling/SlateBrush.h"
#include "Styles/TextAssetUEEditorStyle.h"
#include "DesktopPlatformModule.h"

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

void FAssetTypeActions_TextAssetUE::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) {
  FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

  if (InObjects.Num() == 1) {
    if (UTextAssetUE* TextAsset = Cast<UTextAssetUE>(InObjects[0])) {
      MenuBuilder.AddMenuEntry(
        NSLOCTEXT("TextAssetUE", "ExportToTxt", "Export to .txt"),
        NSLOCTEXT("TextAssetUE", "ExportToTxtTooltip", "Export the text content to a .txt file"),
        FSlateIcon(),
        FUIAction(
          FExecuteAction::CreateLambda([TextAsset]() {
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
                FFileHelper::SaveStringToFile(TextAsset->Content, *SavePath);
              }
            }
            }),
          FCanExecuteAction()
        )
      );
    }
  }
}